
#ifndef ALGOGEN
#define ALGOGEN

#include <deque>
#include <utility>
#include <boost/pool/pool_alloc.hpp>
#include <boost/thread.hpp>

#include "Environnement.hpp"

class AlgoGen
{
public:
	typedef Logique::Environnement Environnement;
	typedef std::deque< Environnement, boost::pool_allocator<Environnement> > EnvironnementList;

	AlgoGen();
	AlgoGen(const std::string& file);
	~AlgoGen();

	void run();
	void stop();

private:
	EnvironnementList _envList;
	boost::thread_group _pool;

	std::string _file;
};

#endif /* !ALGOGEN */