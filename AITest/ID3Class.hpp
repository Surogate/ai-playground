
#ifndef ID3CLASS
#define ID3CLASS

#include <deque>
#include <functional>

#include "Gain.hpp"
#include "TypedTree.hpp"
#include "TreeBuilders.hpp"

template <typename First, typename Second, typename Third, typename Fourth, typename Result>
class ID3Class5 {
public:
	typedef typename TypedTree::Tree5<Result, First, Second, Third, Fourth> Tree;
	typedef typename TreeBuilder::Builder< Tree > Builder;

	ID3Class5() : first_(), second_(), third_(), fourth_(), result_(), actualize_(false), tree_(0)
	{}

	void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Result& res) {
		first_.push_back(val1);
		second_.push_back(val2);
		third_.push_back(val3);
		fourth_.push_back(val4);
		result_.push_back(res);

		actualize_ = false;
	}

	void test() {
		Gain<First, Result> gain1(first_, result_);
		Gain<Second, Result> gain2(second_, result_);
		Gain<Third, Result> gain3(third_, result_);
		Gain<Fourth, Result> gain4(fourth_, result_);

		TypedTree::Debugger<First> d1;
		std::cout << gain1.getResult() << std::endl;

		TypedTree::Debugger<Second> d2;
		std::cout << gain2.getResult() << std::endl;
		
		TypedTree::Debugger<Third> d3;
		std::cout << gain3.getResult() << std::endl;

		TypedTree::Debugger<Fourth> d4;
		std::cout << gain4.getResult() << std::endl;

		typename const Gain<First, Result>::AttribueMap& map = gain1.getAttribueMap();
		typename Gain<First, Result>::AttribueMap::const_iterator it = map.begin();
		typename Gain<First, Result>::AttribueMap::const_iterator ite = map.end();

		TypedTree::Debugger<First> dd1(it->first);
		ConditionnalFunctor1< std::deque<First> > func(first_, it->first);

		func.reset();
		Gain<Second, Result> gg2(second_, result_, func);		

		func.reset();
		Gain<Third, Result> gg3(third_, result_, func);

		func.reset();
		Gain<Fourth, Result> gg4(fourth_, result_, func);

		TypedTree::Debugger<Second> dd2;
		std::cout << gg2.getResult() << std::endl;

		TypedTree::Debugger<Third> dd3;
		std::cout << gg3.getResult() << std::endl;
		
		TypedTree::Debugger<Fourth> dd4;
		std::cout << gg4.getResult() << std::endl;

		typename const Gain<Third, Result>::AttribueMap& map2 = gg3.getAttribueMap();
		typename Gain<Third, Result>::AttribueMap::const_iterator it2 = map2.begin();
		typename Gain<Third, Result>::AttribueMap::const_iterator ite2 = map2.end();

		//while (it2 != ite2) {
		//	TypedTree::Debugger<Third> dd2(it2->first);
		//	++it2;
		//}

		TypedTree::Debugger<Third> ddd3(it2->first);
		ConditionnalFunctorVal< std::deque<Third> > func2(third_, it2->first, func);

		func2.reset();
		Gain<Second, Result> ggg2(second_, result_, func2);

		func2.reset();
		Gain<Fourth, Result> ggg4(fourth_, result_, func2);

		TypedTree::Debugger<Second> ddd2;
		std::cout << ggg2.getResult() << std::endl;
		std::cout << "size " << ggg2.getSize() << std::endl;

		TypedTree::Debugger<Second> ddd4;
		std::cout << ggg4.getResult() << std::endl;
		std::cout << "size " << ggg4.getSize() << std::endl;
	}

	void generateTree() {
		if (tree_ != 0) {
			delete tree_;
			tree_ = 0;
		}
		
		tree_ = Builder::construct(result_, first_, second_, third_, fourth_);
		actualize_ = true;
	}

	Result decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4) {
		return tree_->evaluate(val1, val2, val3, val4);
	}

	Result decideDebug(const First& val1, const Second& val2, const Third& val3, const Fourth& val4) {
		std::cout << "decide Debug" << std::endl;
		return tree_->evaluateDebug(val1, val2, val3, val4);
	}

private:
	std::deque<First> first_;
	std::deque<Second> second_;
	std::deque<Third> third_;
	std::deque<Fourth> fourth_;
	std::deque<Result> result_;

	bool actualize_;
	typename Builder::ATree* tree_;
};


#endif
