
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
