
#include "AlgoGen.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>

AlgoGen::AlgoGen()
	: _envList(), _perfList(), _thread_pool(), _envPool()
	,_run(false), _mut()
	, _file()
{
	BOOST_FOREACH(DoubleArray::value_type& val, _perfList)
	{ val = 0; }
	initRandomEnv();
}

AlgoGen::AlgoGen(const std::string& file)
	: _envList(), _perfList(), _thread_pool(), _envPool()
	,_run(false), _mut()
	, _file(file)
{
	if (!initFromFile())
	{
		BOOST_FOREACH(DoubleArray::value_type& val, _perfList)
		{ val = 0; }
		std::cout << "random seed" << std::endl;
		initRandomEnv();
	}
	else
	{
		std::cout << _envList.size() << " Environnement loaded" << std::endl;
		while (_envList.size() < ENV_SIZE)
		{
			_envList.push_back(make_environnement());
		}
		while (_envList.size() > ENV_SIZE)
		{
			_envList.pop_back();
		}
	}
}

AlgoGen::~AlgoGen()
{
	_envList.clear();
}

void AlgoGen::run()
{
	std::size_t i = 0;
	boost::mt19937 gen;

	BOOST_FOREACH(EnvPtr& e, _envList)
	{
		boost::thread* thd = _thread_pool.create_thread(boost::bind(&Environnement::run, e));
		e->innerThread = thd;
	}
	_run = true;

	while (_run)
	{
		boost::this_thread::sleep(boost::posix_time::minutes(TIMEBEFOREMERGE));
		_mut.lock();
		double total = 0;
		double max = 0;
		std::size_t i = 0;

		BOOST_FOREACH(EnvPtr& e, _envList)
		{
			_perfList[i] = evaluateEnv(*e);
			total += _perfList[i];
			if (_perfList[i] > max)
			{ max = _perfList[i]; }
			++i;
		}

		boost::random::discrete_distribution<> dist(_perfList);
		int pos = dist(gen);
		int pos_2 = dist(gen);
		while (pos_2 == pos)
		{ pos_2 = dist(gen); }

		i = 0;
		while (i < ENV_SIZE)
		{
			_perfList[i] = max - _perfList[i];
			++i;
		}

		int replaced = dist(gen);
		
		_envList[replaced]->stop();
		_envList[replaced]->innerThread->join();
		_envList.push_back(make_environnement(
				EnvironnementGenetic::reproduce(_envList[pos]->getAdn(), _envList[pos_2]->getAdn())
			)
		);
		_envList.erase(_envList.begin() + replaced);

		_envList.back()->innerThread = _thread_pool.create_thread(boost::bind(&Environnement::run, _envList.back()));
		_mut.unlock();
	}
}

void AlgoGen::stop()
{
	_run = false;
	_mut.lock();
	BOOST_FOREACH(EnvPtr& e, _envList)
	{ e->stop(); }
	_thread_pool.join_all();
	dump();
	_mut.unlock();
}

double AlgoGen::evaluateEnv(Environnement& env)
{
	return evaluateEntity(env, &AlgoGen::gatherSheep) * ( 1 + evaluateEntity(env, &AlgoGen::gatherWolf));
}

double AlgoGen::evaluateEntity(Environnement& env, func gatherFunction)
{
	double perfTotal = 0;
	double perfNum = 0;
	double actionTotal = 0;
	double actionNum = 0;

	Environnement::MetricProxy proxy = env.getMetricProxy(); 
	proxy.foreach(boost::bind(gatherFunction, this, _1, 
		boost::ref(perfTotal), boost::ref(perfNum), boost::ref(actionTotal), boost::ref(actionNum)));
	if (perfNum > 0 && actionNum > 0)
		return (perfTotal / perfNum) * (actionTotal / actionNum);
	return 0;
}

void AlgoGen::gatherSheep(const Metric& from, double& perfTotal, double& perfNum, double& actionTotal, double& actionNum)
{
	perfTotal += from.sheepMoy;
	++perfNum;
	actionTotal += from.sheepActionNeural;
	actionNum += from.sheepActionNum;
}

void AlgoGen::gatherWolf(const Metric& from, double& perfTotal, double& perfNum, double& actionTotal, double& actionNum)
{
	perfTotal += from.wolfMoy;
	++perfNum;
	actionTotal += from.wolfActionNeural;
	actionNum += from.wolfActionNum;
}

void AlgoGen::initRandomEnv()
{
	for (std::size_t i = 0; i < ENV_SIZE; ++i)
	{ _envList.push_back(make_environnement()); }
}

bool AlgoGen::initFromFile()
{
	bool valid = false;
	if (_file.size())
	{
		std::ifstream stream;
		stream.open(_file.c_str(), std::ios::in |std::ios::beg);
		EnvironnementGenetic adnTmp;
		DoubleArray::value_type perfTmp;
		std::size_t perf_index = 0;
		do
		{
			stream >> adnTmp;
			stream >> perfTmp;
			if (stream.good())
			{
				valid = true;
				_envList.push_back(make_environnement(adnTmp));
				_perfList[perf_index] = perfTmp;
				++perf_index;
			}
		} while (stream.good());
	}
	return valid;
}

void AlgoGen::dump()
{
	if (_file.size())
	{
		std::ofstream stream;
		std::size_t perf_index = 0;
		stream.open(_file.c_str(), std::ios::out | std::ios::trunc);
		if (stream.good())
		{
			BOOST_FOREACH(EnvPtr& e, _envList)
			{
				e->dump(stream);
				stream << " " << _perfList[perf_index] << std::endl;
				++perf_index;
			}
		}
	}
}

AlgoGen::EnvPtr AlgoGen::make_environnement()
{
	return make_environnement(EnvironnementGenetic::randomGen());
}

AlgoGen::EnvPtr AlgoGen::make_environnement(EnvironnementGenetic& genetic)
{
	//return EnvPtr(new Environnement(genetic));
	return EnvPtr(_envPool.construct(genetic), boost::bind(&AlgoGen::destroy_environnement, this, _1));
}

void AlgoGen::destroy_environnement(Environnement* env)
{
	//delete env;
	_envPool.destroy(env);
}

