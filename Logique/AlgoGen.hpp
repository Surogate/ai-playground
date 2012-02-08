
#ifndef ALGOGEN
#define ALGOGEN

#include <deque>
#include <utility>
#include <boost/pool/pool_alloc.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "Environnement.hpp"

class AlgoGen
{
public:
	enum {
		ENV_SIZE = 20
	};

	typedef Logique::Environnement Environnement;
	typedef std::deque< boost::shared_ptr<Environnement> > EnvironnementArray;
	typedef boost::array< double, ENV_SIZE > DoubleArray;
	typedef void (AlgoGen::*func)(const Metric&, double& , double& , double&, double&);

	AlgoGen();
	AlgoGen(const std::string& file);
	~AlgoGen();

	void run();
	void stop();
	void gatherSheep(const Metric& from, double& perfTotal, double& perfNum, double& actienTotal, double& actionNum);
	void gatherWolf(const Metric& from, double& perfTotal, double& perfNum, double& actienTotal, double& actionNum);
private:
	void initRandomEnv();
	double evaluateEnv(Environnement& env);
	double evaluateEntity(Environnement& env, func gatherFunction);
	double evaluateWolf(Environnement& env);

	void dump();
	bool initFromFile();

	EnvironnementArray _envList;
	DoubleArray _perfList;
	boost::thread_group _thread_pool;
	bool _run;
	std::string _file;
};

#endif /* !ALGOGEN */