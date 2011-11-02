/* 
 * File:   ID3Class.hpp
 * Author: Francois Ancel (francoisancel [at] gmail.com)
 * Copyright : BSD license
 * Created on September 16, 2011
 */

#ifndef ID3CLASS
#define ID3CLASS

#include <deque>
#include <boost/function.hpp>

#include "Gain.hpp"
#include "TypedTree.hpp"
#include "TreeBuilders.hpp"

namespace ID3 {

	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth, typename Result>
	class ID3Class10 {
	public:
		typedef typename TypedTree::Tree10<Result, First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class10() : _first(), _second(), _third(), _fourth(), _fifth(), _sixth(), _seventh(), _eighth(), _ninth(), _result(), _actualize(false), _tree(0)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Ninth& val9, const Result& res) {
			_first.push_back(val1);
			_second.push_back(val2);
			_third.push_back(val3);
			_fourth.push_back(val4);
			_fifth.push_back(val5);
			_sixth.push_back(val6);
			_seventh.push_back(val7);
			_eighth.push_back(val8);
			_ninth.push_back(val9);
			_result.push_back(res);

			_actualize = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			_getRandomResult = functor;
		}

		void generateTree() {
			if (!_actualize) {
				if (_tree != 0) {
					delete _tree;
					_tree = 0;
				}
		
				if (_result.size()) {
					std::cout << "build tree" << std::endl;
					_tree = TreeBuilder::Starter<Tree>::construct(_result, _first, _second, _third, _fourth, _fifth, _sixth, _seventh, _eighth, _ninth);
				}
				_actualize = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Ninth& val9) {
			if (_tree) {
				return _tree->evaluate(val1, val2, val3, val4, val5, val6, val7, val8, val9);
			} else if (_getRandomResult){
				return _getRandomResult();
			}
			return Answer();
		}

		std::size_t size() const {
			return _result.size();
		}

	private:
		std::deque<First> _first;
		std::deque<Second> _second;
		std::deque<Third> _third;
		std::deque<Fourth> _fourth;
		std::deque<Fifth> _fifth;
		std::deque<Sixth> _sixth;
		std::deque<Seventh> _seventh;
		std::deque<Eighth> _eighth;
		std::deque<Ninth> _ninth;
		std::deque<Result> _result;

		typename Builder::ATree* _tree;
		boost::function< Result() > _getRandomResult;
		bool _actualize;
	};

	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Result>
	class ID3Class9 {
	public:
		typedef typename TypedTree::Tree9<Result, First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class9() : _first(), _second(), _third(), _fourth(), _fifth(), _sixth(), _seventh(), _eighth(), _result(), _actualize(false), _tree(0)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8, const Result& res) {
			_first.push_back(val1);
			_second.push_back(val2);
			_third.push_back(val3);
			_fourth.push_back(val4);
			_fifth.push_back(val5);
			_sixth.push_back(val6);
			_seventh.push_back(val7);
			_eighth.push_back(val8);
			_result.push_back(res);

			_actualize = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			_getRandomResult = functor;
		}

		void generateTree() {
			if (!_actualize) {
				if (_tree != 0) {
					delete _tree;
					_tree = 0;
				}
		
				if (result_.size()) {
					_tree = TreeBuilder::Starter<Tree>::construct(_result, _first, _second, _third, _fourth, _fifth, _sixth, _seventh, _eighth);
				}
				_actualize = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Eighth& val8) {
			if (_tree) {
				return _tree->evaluate(val1, val2, val3, val4, val5, val6, val7, val8);
			} else if (_getRandomResult){
				return _getRandomResult();
			}
			return Answer();
		}

		std::size_t size() const {
			return _result.size();
		}

	private:
		std::deque<First> _first;
		std::deque<Second> _second;
		std::deque<Third> _third;
		std::deque<Fourth> _fourth;
		std::deque<Fifth> _fifth;
		std::deque<Sixth> _sixth;
		std::deque<Seventh> _seventh;
		std::deque<Eighth> _eighth;
		std::deque<Result> _result;

		typename Builder::ATree* _tree;
		boost::function< Result() > _getRandomResult;
		bool _actualize;
	};
	
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Result>
	class ID3Class8 {
	public:
		typedef typename TypedTree::Tree8<Result, First, Second, Third, Fourth, Fifth, Sixth, Seventh> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class8() : _first(), _second(), _third(), _fourth(), _fifth(), _sixth(), _seventh(), _result(), _actualize(false), _tree(0)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7, const Result& res) {
			_first.push_back(val1);
			_second.push_back(val2);
			_third.push_back(val3);
			_fourth.push_back(val4);
			_fifth.push_back(val5);
			_sixth.push_back(val6);
			_seventh.push_back(val7);
			_result.push_back(res);

			_actualize = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			_getRandomResult = functor;
		}

		void generateTree() {
			if (!_actualize) {
				if (_tree != 0) {
					delete _tree;
					_tree = 0;
				}
		
				if (result_.size()) {
					_tree = TreeBuilder::Starter<Tree>::construct(_result, _first, _second, _third, _fourth, _fifth, _sixth, _seventh);
				}
				_actualize = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Seventh& val7) {
			if (_tree) {
				return _tree->evaluate(val1, val2, val3, val4, val5, val6, val7);
			} else if (_getRandomResult){
				return _getRandomResult();
			}
			return Answer();
		}

		std::size_t size() const {
			return _result.size();
		}

	private:
		std::deque<First> _first;
		std::deque<Second> _second;
		std::deque<Third> _third;
		std::deque<Fourth> _fourth;
		std::deque<Fifth> _fifth;
		std::deque<Sixth> _sixth;
		std::deque<Seventh> _seventh;
		std::deque<Result> _result;

		typename Builder::ATree* _tree;
		boost::function< Result() > _getRandomResult;
		bool _actualize;
	};

	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Result>
	class ID3Class7 {
	public:
		typedef typename TypedTree::Tree7<Result, First, Second, Third, Fourth, Fifth, Sixth> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class7() : first_(), second_(), third_(), fourth_(), fifth_(), sixth_(), result_(), actualize_(false), tree_(0)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6, const Result& res) {
			first_.push_back(val1);
			second_.push_back(val2);
			third_.push_back(val3);
			fourth_.push_back(val4);
			fifth_.push_back(val5);
			sixth_.push_back(val6);
			result_.push_back(res);

			actualize_ = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			getRandomResult_ = functor;
		}

		void generateTree() {
			if (!actualize_) {
				if (tree_ != 0) {
					delete tree_;
					tree_ = 0;
				}
		
				if (result_.size()) {
					tree_ = TreeBuilder::Starter<Tree>::construct(result_, first_, second_, third_, fourth_, fifth_, sixth_);
				}
				actualize_ = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Sixth& val6) {
			if (tree_) {
				return tree_->evaluate(val1, val2, val3, val4, val5, val6);
			} else if (getRandomResult_){
				return getRandomResult_();
			}
			return Answer();
		}

		std::size_t size() const {
			return result_.size();
		}

	private:
		std::deque<First> first_;
		std::deque<Second> second_;
		std::deque<Third> third_;
		std::deque<Fourth> fourth_;
		std::deque<Fifth> fifth_;
		std::deque<Sixth> sixth_;
		std::deque<Result> result_;

		typename Builder::ATree* tree_;
		boost::function< Result() > getRandomResult_;
		bool actualize_;
	};

	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Result>
	class ID3Class6 {
	public:
		typedef typename TypedTree::Tree6<Result, First, Second, Third, Fourth, Fifth> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class6() 
			: first_(), second_(), third_(), fourth_(), fifth_(), result_(), tree_(0), actualize_(false)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5, const Result& res) {
			first_.push_back(val1);
			second_.push_back(val2);
			third_.push_back(val3);
			fourth_.push_back(val4);
			fifth_.push_back(val5);
			result_.push_back(res);

			actualize_ = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			getRandomResult_ = functor;
		}

		void generateTree() {
			if (!actualize_) {
				if (tree_ != 0) {
					delete tree_;
					tree_ = 0;
				}
		
				if (result_.size()) {
					tree_ = TreeBuilder::Starter<Tree>::construct(result_, first_, second_, third_, fourth_, fifth_);
				}
				actualize_ = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Fifth& val5) {
			if (tree_) {
				return tree_->evaluate(val1, val2, val3, val4, val5);
			} else if (getRandomResult_){
				return getRandomResult_();
			}
			return Answer();
		}

		std::size_t size() const {
			return result_.size();
		}

	private:
		std::deque<First> first_;
		std::deque<Second> second_;
		std::deque<Third> third_;
		std::deque<Fourth> fourth_;
		std::deque<Fifth> fifth_;
		std::deque<Result> result_;

		typename Builder::ATree* tree_;
		boost::function< Result() > getRandomResult_;
		bool actualize_;
	};

	template <typename First, typename Second, typename Third, typename Fourth, typename Result>
	class ID3Class5 {
	public:
		typedef typename TypedTree::Tree5<Result, First, Second, Third, Fourth> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class5() : first_(), second_(), third_(), fourth_(), result_(), tree_(0), actualize_(false)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Fourth& val4, const Result& res) {
			first_.push_back(val1);
			second_.push_back(val2);
			third_.push_back(val3);
			fourth_.push_back(val4);
			result_.push_back(res);

			actualize_ = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			getRandomResult_ = functor;
		}

		void generateTree() {
			if (!actualize_) {
				if (tree_ != 0) {
					delete tree_;
					tree_ = 0;
				}
		
				if (result_.size()) {
					tree_ = TreeBuilder::Starter<Tree>::construct(result_, first_, second_, third_, fourth_);
				}
				actualize_ = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3, const Fourth& val4) {
			if (tree_) {
				return tree_->evaluate(val1, val2, val3, val4);
			} else if (getRandomResult_){
				return getRandomResult_();
			}
			return Answer();
		}

		std::size_t size() const {
			return result_.size();
		}

	private:
		std::deque<First> first_;
		std::deque<Second> second_;
		std::deque<Third> third_;
		std::deque<Fourth> fourth_;
		std::deque<Result> result_;

		typename Builder::ATree* tree_;
		boost::function< Result() > getRandomResult_;
		bool actualize_;
	};

	template <typename First, typename Second, typename Third, typename Result>
	class ID3Class4 {
	public:
		typedef typename TypedTree::Tree4<Result, First, Second, Third> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class4() : first_(), second_(), third_(), result_(), tree_(0), actualize_(false)
		{}

		void addData(const First& val1, const Second& val2, const Third& val3, const Result& res) {
			first_.push_back(val1);
			second_.push_back(val2);
			third_.push_back(val3);
			result_.push_back(res);

			actualize_ = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			getRandomResult_ = functor;
		}

		void generateTree() {
			if (!actualize_) {
				if (tree_ != 0) {
					delete tree_;
					tree_ = 0;
				}
		
				if (result_.size()) {
					tree_ = TreeBuilder::Starter<Tree>::construct(result_, first_, second_, third_);
				}
				actualize_ = true;
			}
		}

		Answer decide(const First& val1, const Second& val2, const Third& val3) {
			if (tree_) {
				return tree_->evaluate(val1, val2, val3);
			} else if (getRandomResult_){
				return getRandomResult_();
			}
			return Answer();
		}

		std::size_t size() const {
			return result_.size();
		}

	private:
		std::deque<First> first_;
		std::deque<Second> second_;
		std::deque<Third> third_;
		std::deque<Result> result_;

		typename Builder::ATree* tree_;
		boost::function< Result() > getRandomResult_;
		bool actualize_;
	};

	template <typename First, typename Second, typename Result>
	class ID3Class3 {
	public:
		typedef typename TypedTree::Tree3<Result, First, Second> Tree;
		typedef typename Tree::Answer Answer;
		typedef typename TreeBuilder::Builder< Tree > Builder;

		ID3Class3() : first_(), second_(), result_(), tree_(0), actualize_(false)
		{}

		void addData(const First& val1, const Second& val2, const Result& res) {
			first_.push_back(val1);
			second_.push_back(val2);
			result_.push_back(res);

			actualize_ = false;
		}

		void setRandomResult(const boost::function< Result() >& functor) {
			getRandomResult_ = functor;
		}

		void generateTree() {
			if (!actualize_) {
				if (tree_ != 0) {
					delete tree_;
					tree_ = 0;
				}
		
				if (result_.size()) {
					tree_ = TreeBuilder::Starter<Tree>::construct(result_, first_, second_);
				}
				actualize_ = true;
			}
		}

		Answer decide(const First& val1, const Second& val2) {
			if (tree_) {
				return tree_->evaluate(val1, val2);
			} else if (getRandomResult_){
				return getRandomResult_();
			}
			return Answer();
		}

		std::size_t size() const {
			return result_.size();
		}

	private:
		std::deque<First> first_;
		std::deque<Second> second_;
		std::deque<Result> result_;

		typename Builder::ATree* tree_;
		boost::function< Result() > getRandomResult_;
		bool actualize_;
	};

}

#endif
