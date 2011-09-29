
#ifndef CONDITIONNALFUNCTOR_HPP
#define CONDITIONNALFUNCTOR_HPP

class ConditionnalFunctor {
public:
	virtual ~ConditionnalFunctor() {}
	virtual bool operator()() const = 0;
	virtual void operator++() = 0;
};

template <typename T, typename Y>
class ConditionnalFunctor1 : public ConditionnalFunctor {
public:
	ConditionnalFunctor1(const T& container, const Y& value) 
		: it_(container.begin()), ite_(container.end()), value_(value) 
	{}

	bool operator()() const {
		return it_ != ite_ && *it_ == value_;
	}

	void operator++() {
		if (it_ != ite_)
			++it_;
	}

private:
	typename T::const_iterator it_;
	typename T::const_iterator ite_;
	const Y& value_;
};

template <typename T, typename Y>
class ConditionnalFunctorVal : public ConditionnalFunctor {
	public:
	ConditionnalFunctorVal(const T& container, const Y& value, ConditionnalFunctor& buddy) 
		: it_(container.begin()), ite_(container.end()), value_(value), buddy_(buddy)
	{}

	bool operator()() const {
		return it_ != ite_ && *it_ == value_ && buddy_();
	}

	void operator++() {
		if (it_ != ite_)
			++it_;
		++buddy_;
	}

private:
	typename T::const_iterator it_;
	typename T::const_iterator ite_;
	const Y& value_;
	ConditionnalFunctor& buddy_;
};

template <typename T, typename Y, typename U, typename I>
class ConditionnalFunctor2 : public ConditionnalFunctor {
public:
	ConditionnalFunctor2(const T& container, const Y& value, const U& container2, const I& value2) 
		: it_(container.begin()), ite_(container.end()), it2_(container2.begin()), ite2_(container2.end()),  value_(value), value2_(value2)
	{}

	bool operator()() const {
		return it_ != ite_ && *it_ == value_ && it2_ != ite2_ && *it2_ == value2_;
	}

	void operator++() {
		if (it_ != ite_)
			++it_;
		if (it2_ != ite2_)
			++it2_;
	}

private:
	typename T::const_iterator it_;
	typename T::const_iterator ite_;
	typename U::const_iterator it2_;
	typename U::const_iterator ite2_;
	const Y& value_;
	const I& value2_;
};

template <typename T, typename Y, typename U, typename I, typename O, typename P>
class ConditionnalFunctor3 : public ConditionnalFunctor {
public:
	ConditionnalFunctor3(const T& container, const Y& value, const U& container2, const I& value2, const O& container3, const P& value3) 
		: it_(container.begin()), ite_(container.end()), 
		it2_(container2.begin()), ite2_(container2.end()), 
		it3_(container3.begin()), ite3_(container3.end()), 
		value_(value), value2_(value2), value3_(value3)
	{}

	bool operator()() const {
		return it_ != ite_ && *it_ == value_ && it2_ != ite2_ && *it2_ == value2_ && it3_ != ite3_ && *it3_ == value3_;
	}

	void operator++() {
		if (it_ != ite_)
			++it_;
		if (it2_ != ite2_)
			++it2_;
	}

private:
	typename T::const_iterator it_;
	typename T::const_iterator ite_;
	typename U::const_iterator it2_;
	typename U::const_iterator ite2_;
	typename O::const_iterator it3_;
	typename O::const_iterator ite3_;

	const Y& value_;
	const I& value2_;
	const P& value3_;
};



#endif /* CONDITIONNALFUNCTOR_HPP */