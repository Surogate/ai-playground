/* 
 * File:   ConditionnalFunctor.hpp
 * Author: Francois Ancel (francoisancel [at] gmail.com)
 * Copyright : BSD license
 * Created on September 16, 2011
 */

#ifndef CONDITIONNALFUNCTOR_HPP
#define CONDITIONNALFUNCTOR_HPP

/*
	ConditionnalFunctor a used to reduce dataset size during Gain calculation.
*/

class ConditionnalFunctor {
public:
	virtual ~ConditionnalFunctor() {}
	virtual bool operator()() const = 0;
	virtual void operator++() = 0;
	virtual void reset() = 0;
	virtual void debug() = 0;
};


template <typename AttributeContainer>
class ConditionnalFunctor1 : public ConditionnalFunctor {
public:
	typedef typename AttributeContainer::value_type Attribute;

	ConditionnalFunctor1(const AttributeContainer& container, const Attribute& value) 
		: it_(container.begin()), tmp_(it_), ite_(container.end()), value_(value), buddy_(0)
	{}

	ConditionnalFunctor1(const AttributeContainer& container, const Attribute& value, ConditionnalFunctor* buddy) 
		: it_(container.begin()), tmp_(it_), ite_(container.end()), value_(value), buddy_(buddy) 
	{}

	bool operator()() const {
		bool result = tmp_ != ite_ && *tmp_ == value_;
		if (buddy_)
			return result && ((*buddy_)());
		return result;
	}

	void operator++() {
		if (tmp_ != ite_)
			++tmp_;
		if (buddy_) buddy_->operator++();
	}

	void reset() {
		tmp_ = it_;
		if (buddy_) buddy_->reset();
	}

	void debug() {
		#ifdef DEBUG_CONDITIONNALFUNCTOR
		std::cout << "Functor value" << std::endl;
		TypedTree::Debugger<Attribute> debug(value_);
		if (buddy_) buddy_->debug();
		#endif
	}

private:
	typename AttributeContainer::const_iterator it_;
	typename AttributeContainer::const_iterator tmp_;
	typename AttributeContainer::const_iterator ite_;
	Attribute value_;
	ConditionnalFunctor* buddy_;
};

template <typename AttributeContainer>
class ConditionnalFunctorVal : public ConditionnalFunctor {
public:
	typedef typename AttributeContainer::value_type Attribute;

	ConditionnalFunctorVal(const AttributeContainer& container, const Attribute& value, ConditionnalFunctor& buddy) 
		: it_(container.begin()), tmp_(it_), ite_(container.end()), value_(value), buddy_(buddy)
	{}

	bool operator()() const {
		return tmp_ != ite_ && *tmp_ == value_ && buddy_();
	}

	void operator++() {
		if (tmp_ != ite_)
			++tmp_;
		++buddy_;
	}

	void reset() {
		tmp_ = it_;
		buddy_.reset();
	}

	void debug() {
		#ifdef DEBUG_CONDITIONNALFUNCTOR
		std::cout << "Functor value" << std::endl;
		TypedTree::Debugger<Attribute> debug(value_);
		buddy_.debug();
		#endif
	}

private:
	typename AttributeContainer::const_iterator it_;
	typename AttributeContainer::const_iterator tmp_;
	typename AttributeContainer::const_iterator ite_;
	Attribute value_;
	ConditionnalFunctor& buddy_;
};

#endif /* CONDITIONNALFUNCTOR_HPP */