
#include "AlgoGen.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>

AlgoGen::AlgoGen()
	: _envList(), _run(false)
	, _file()
{
	initRandomEnv();
}

AlgoGen::AlgoGen(const std::string& file)
	: _envList(), _run(false)
	, _file(file)
{
	if (!initFromFile())
	{
		initRandomEnv();
	}
}

AlgoGen::~AlgoGen()
{}

void AlgoGen::run()
{
	std::size_t i = 0;
	boost::mt19937 gen;

	BOOST_FOREACH(boost::shared_ptr<Environnement>& e, _envList)
	{
		boost::thread* thd = _thread_pool.create_thread(boost::bind(&Environnement::run, e));
		e->innerThread = thd;
	}

	_run = true;
	while (_run)
	{
		boost::this_thread::sleep(boost::posix_time::minutes(30));
		double total = 0;
		double max = 0;
		std::size_t i = 0;

		BOOST_FOREACH(boost::shared_ptr<Environnement>& e, _envList)
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
		_envList.erase(_envList.begin() + replaced);
		_envList.push_back(boost::make_shared<Environnement>(
				EnvironnementGenetic::reproduce(_envList[pos]->getAdn(), _envList[pos_2]->getAdn())
			)
		);

		_envList.back()->innerThread = _thread_pool.create_thread(boost::bind(&Environnement::run, _envList.back()));
	}
}

void AlgoGen::stop()
{
	_run = false;
	BOOST_FOREACH(boost::shared_ptr<Environnement>& e, _envList)
	{ e->stop(); }
	BOOST_FOREACH(boost::shared_ptr<Environnement>& e, _envList)
	{ e->innerThread->join(); }
	dump();
}

void AlgoGen::dump()
{
	std::ofstream stream;
	stream.open(_file.c_str(), std::ios::out | std::ios::trunc);
	if (!stream.bad())
	{
		BOOST_FOREACH(boost::shared_ptr<Environnement>& e, _envList)
		{ e->dump(stream); }
		stream.close();
	}
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
	{ _envList.push_back(boost::make_shared<Environnement>(EnvironnementGenetic::randomGen())); }
}

bool AlgoGen::initFromFile()
{
	return false;
}
