
#include "AlgoGen.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

AlgoGen::AlgoGen()
	: _envList(), _pool()
	, _file()
{}

AlgoGen::AlgoGen(const std::string& file)
	: _envList(), _pool()
	, _file(file)
{}

void AlgoGen::run()
{
	BOOST_FOREACH(Environnement& e, _envList)
	{
		_pool.create_thread(boost::bind(&Environnement::run, &e));
	}
}

void AlgoGen::stop()
{
	BOOST_FOREACH(Environnement& e, _envList)
	{
		e.stop();
	}
}
