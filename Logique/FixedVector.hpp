
#ifndef FIXEDVECTOR_HPP
#define FIXEDVECTOR_HPP

#include <cstddef>
#include <exception>

template < typename T, std::size_t MAX_SIZE >
class FixedVector
{
public:
	//Definition

	typedef T value_type;
	typedef std::size_t size_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef ptrdiff_t difference_type;

	enum {
		MAX_ELEMENT = MAX_SIZE
	};

	//Internal class

	class OutOfBoundException : std::exception {
	public:
		OutOfBoundException() throw() {}
		~OutOfBoundException()  throw() {}
		const char* what() const throw() {
			return "Out of bound";
		}
	};

	class iterator {
	private:
		T* _data;
		size_type _pos;

	public:
		iterator() 
			: _data(0), _pos(0)
		{}

		iterator(T* data, size_type pos)
			: _data(data), _pos(pos)
		{}

		iterator(const iterator& orig)
			: _data(orig._data), _pos(orig._pos)
		{}

		~iterator() {}

		iterator& operator=(const iterator& orig)
		{
			if (this != &orig)
			{
				_data = orig._data;
				_pos = orig._pos;
			}
			return *this;
		}

		reference operator*() 
		{
			return _data[_pos];
		}

		void operator++()
		{
			++_pos;
		}

		void operator--()
		{
			--_pos;
		}

		bool operator!=(const iterator& buddy)
		{
			return _pos != buddy._pos;
		}

		bool operator==(const iterator& buddy)
		{
			return _pos == buddy._pos;
		}

		bool operator<(const iterator& buddy)
		{
			return _pos < buddy._pos;
		}

		bool operator>(const iterator& buddy)
		{
			return _pos > buddy._pos;
		}

		bool operator<=(const iterator& buddy)
		{
			return _pos <= buddy._pos;
		}
		
		bool operator>=(const iterator& buddy)
		{
			return _pos >= buddy._pos;
		}

		void operator+=(size_type n)
		{
			_pos += n;
		}

		void operator-=(size_type n)
		{
			_pos -= n;
		}

		reference operator[](size_type n)
		{
			return _data[_pos + n];
		}
	};

	class const_iterator {
	private:
		const T* _data;
		size_type _pos;

	public:
		const_iterator() 
			: _data(0), _pos(0)
		{}

		const_iterator(const T* data, size_type pos)
			: _data(data), _pos(pos)
		{}

		const_iterator(const const_iterator& orig)
			: _data(orig._data), _pos(orig._pos)
		{}

		~const_iterator() {}

		const_iterator& operator=(const const_iterator& orig)
		{
			if (this != &orig)
			{
				_data = orig._data;
				_pos = orig._pos;
			}
			return *this;
		}

		const reference operator*() 
		{
			return _data[_pos];
		}

		void operator++()
		{
			++_pos;
		}

		void operator--()
		{
			--_pos;
		}

		bool operator!=(const const_iterator& buddy)
		{
			return _pos != buddy._pos;
		}

		bool operator==(const const_iterator& buddy)
		{
			return _pos == buddy._pos;
		}

		bool operator<(const const_iterator& buddy)
		{
			return _pos < buddy._pos;
		}

		bool operator>(const const_iterator& buddy)
		{
			return _pos > buddy._pos;
		}

		bool operator<=(const const_iterator& buddy)
		{
			return _pos <= buddy._pos;
		}
		
		bool operator>=(const const_iterator& buddy)
		{
			return _pos >= buddy._pos;
		}

		void operator+=(size_type n)
		{
			_pos += n;
		}

		void operator-=(size_type n)
		{
			_pos -= n;
		}

		const_reference operator[](size_type n)
		{
			return _data[_pos + n];
		}
	};

	//Constructor

	FixedVector()
		: _size(0)
	{}

	FixedVector(size_type n, const T& value= T())
		: _data(), _size(n)
	{
		for (size_type i = 0; i < _size; ++i)
		{
			_data[i] = value;
		}
	}

	template <class InputIterator>
	FixedVector( InputIterator first, InputIterator last)
	{
		size_type i = 0;
		while (first != last && i < MAX_SIZE)
		{
			_data[i] = *first;
			++first;
			++i;
		}

		if (i == MAX_SIZE)
			throw OutOfBoundException();
		_size = i;
	}

	FixedVector(const FixedVector& orig)
		: _size(orig._size)
	{
		for (unsigned int i = 0; i < orig._size; ++i)
			_data[i] = orig._data[i];
	}

	~FixedVector()
	{}

	FixedVector& operator=(const FixedVector& orig)
	{
		if (this != &orig)
		{
			for (size_type i = 0; i < orig.size(); ++i)
				_data[i] = orig._data[i];
		}
		return *this;
	}

	//Iterator

	iterator begin() 
	{
		return iterator(_data, 0);
	}

	const_iterator begin() const 
	{
		return const_iterator(_data, 0);
	}

	iterator end()
	{
		return iterator(_data, _size);
	}

	const_iterator end() const
	{
		return const_iterator(_data, _size);
	}

	//Capacity
	inline size_type size() const
	{
		return _size;
	}

	inline size_type max_size() const
	{
		return MAX_SIZE;
	}

	void resize(size_type sz, T c = T())
	{
		if (sz > max_size())
			throw OutOfBoundException();
		if (sz <= _size)
			_size = sz;
		if (sz > _size)
		{
			for (unsigned int i = _size; i < sz; ++i) 
			{
				_data[i] = c;
			}
			_size = sz;
		}
	}

	size_type capacity() const 
	{
		return max_size();
	}

	bool empty() const
	{
		return _size == 0;
	}

	void reserve(size_type n)
	{
		check_size(n);
	}

	//Element access

	reference operator[](size_type n)
	{
		check_size(n);
		return _data[n];
	}

	const_reference operator[](size_type n) const
	{
		check_size(n);
		return _data[n];
	}

	reference at(size_type n)
	{
		check_size(n);
		return _data[n];
	}

	const_reference at(size_type n) const
	{
		check_size(n);
		return _data[n];
	}

	reference front()
	{
		check_empty();
		return _data[0];
	}

	const_reference front() const
	{
		check_empty();
		return _data[0];
	}

	reference back()
	{
		check_empty();
		return _data[_size - 1];
	}

	const_reference back() const
	{
		check_empty();
		return _data[_size - 1];
	}

	//Modifier

	template <class InputIterator>
	void assign ( InputIterator first, InputIterator last )
	{
		size_type i = 0;
		while (first != last && i < MAX_SIZE)
		{
			_data[i] = *first;
			++first;
			++i;
		}

		if (first != last)
			throw OutOfBoundException();
	}

	void assign ( size_type n, const T& u )
	{
		check_size(n);
		for (size_type i = 0; i < n; ++i)
		{
			_data[i] = u;
		}
	}

	void push_back ( const T& x )
	{
		check_size(_size + 1);
		_data[_size] = x;
		_size++;
	}

	void pop_back()
	{
		check_empty();
		_size--;
	}

	iterator insert ( iterator position, const T& x )
	{
		check_iterator(position);
		check_size(_size + 1);
		for (size_type i = _size; i > position._pos; --i)
		{
			_data[i] = _data[i - 1];
		}
		_data[position._pos] = x;
		_size++;
	}

    void insert ( iterator position, size_type n, const T& x )
	{
		for (size_type i = 0; i < n; ++i)
		{
			insert(position, x);
		}
	}

	template <class InputIterator>
	void insert ( iterator position, InputIterator first, InputIterator last ) 
	{
		while (first != last)
		{
			insert(position, *first);
		}
	}

	iterator erase ( iterator position )
	{
		check_iterator(position);
		for (size_type i = position._pos; i < _size - 1; ++i)
		{
			_data[i] = _data[i + 1];
		}
		_size--;
		return position;
	}

	iterator erase ( iterator first, iterator last )
	{
		check_iterator(first);
		iterator tmp = first;
		while (first != last) 
		{
			erase(tmp);
			first++;
		}
		return tmp;
	}

	void swap( FixedVector& value )
	{
		T* dataTmp = _data;
		size_type sizeTmp = _size;

		_data = value._data;
		_size = value._size;
		value._data = dataTmp;
		value._size = sizeTmp;
	}

	void clear()
	{
		_size = 0;
	}

private:
	inline void check_size(size_type n) const
	{
		if (n >= max_size())
			throw OutOfBoundException();
	}

	inline void check_empty() const
	{
		if (_size == 0)
			throw OutOfBoundException();
	}

	inline void check_iterator(iterator& it) const 
	{
		check_size(it._pos);
	}

	size_type _size;
	T _data[MAX_SIZE];	
};

#endif