
#ifndef CONDITIONNALFUNCTOR_HPP
#define CONDITIONNALFUNCTOR_HPP

class ConditionnalFunctor {
public:
	virtual ~ConditionnalFunctor() {}
	virtual bool operator()() const = 0;
	virtual void operator++() = 0;
	virtual void reset() = 0;
};

template <typename T, typename Y>
class ConditionnalFunctor1 : public ConditionnalFunctor {
public:
	ConditionnalFunctor1(const T& container, const Y& value) 
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

private:
	typename T::const_iterator it_;
	typename T::const_iterator tmp_;
	typename T::const_iterator ite_;
	Y value_;
};

template <typename T, typename Y>
class ConditionnalFunctorVal : public ConditionnalFunctor {
	public:
	ConditionnalFunctorVal(const T& container, const Y& value, ConditionnalFunctor& buddy) 
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

private:
	typename T::const_iterator it_;
	typename T::const_iterator tmp_;
	typename T::const_iterator ite_;
	Y value_;
	ConditionnalFunctor& buddy_;
};

#endif /* CONDITIONNALFUNCTOR_HPP */