
#ifndef CONDITIONNALFUNCTOR_HPP
#define CONDITIONNALFUNCTOR_HPP

class ConditionnalFunctor {
public:
	virtual ~ConditionnalFunctor() {}
	virtual bool operator()() const = 0;
	virtual void operator++() = 0;
	virtual void reset() = 0;
	virtual void debug() = 0;
};

template <typename T>
class ConditionnalFunctor1 : public ConditionnalFunctor {
public:
	typedef typename T::value_type value_type;

	ConditionnalFunctor1(const T& container, const value_type& value) 
		: it_(container.begin()), tmp_(it_), ite_(container.end()), value_(value) 
	{}

	bool operator()() const {
		return tmp_ != ite_ && *tmp_ == value_;
	}

	void operator++() {
		if (tmp_ != ite_)
			++tmp_;
	}

	void reset() {
		tmp_ = it_;
	}

	void debug() {
		std::cout << "Functor value" << std::endl;
		TypedTree::Debugger<value_type> debug(value_);
	}

private:
	typename T::const_iterator it_;
	typename T::const_iterator tmp_;
	typename T::const_iterator ite_;
	value_type value_;
};

template <typename T>
class ConditionnalFunctorVal : public ConditionnalFunctor {
public:
	typedef typename T::value_type value_type;

	ConditionnalFunctorVal(const T& container, const value_type& value, ConditionnalFunctor& buddy) 
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
		std::cout << "Functor value" << std::endl;
		TypedTree::Debugger<value_type> debug(value_);
		buddy_.debug();
	}

private:
	typename T::const_iterator it_;
	typename T::const_iterator tmp_;
	typename T::const_iterator ite_;
	value_type value_;
	ConditionnalFunctor& buddy_;
};

#endif /* CONDITIONNALFUNCTOR_HPP */