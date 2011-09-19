/* 
 * File:   Array.hpp
 * Author: fancel
 *
 * Created on September 16, 2011, 12:52 PM
 */

#ifndef ARRAY_HPP
#define	ARRAY_HPP

template < typename T, unsigned int SIZE >
class Array {
private:
	T data_[SIZE];

public:
	typedef T value_type;

	unsigned int length() {
		return SIZE;
	}

	unsigned int size() {
		return SIZE;
	}

	const T & operator[](unsigned int index) const {
		return data_[index];
	}

	T & operator[](unsigned int index) {
		return data_[index];
	}

	class iterator {
	private:
		unsigned int index_;
		T* data_;

	public:
		iterator(unsigned int index, T* data) : index_(index), data_(data)
		{}

		void operator++() {
			index_++;
		}

		void operator--() {
			index_--;
		}

		bool operator!=(const iterator& value) {
			return index_ != value.index_;
		}

		T& operator*() {
			return data_[index_];
		}
	};

	class const_iterator {
	private:
		unsigned int index_;
		T* data_;

	public:
		const_iterator(unsigned int index, T* data) : index_(index), data_(data)
		{}

		void operator++() {
			index_++;
		}

		void operator--() {
			index_--;
		}

		bool operator!=(const iterator& value) {
			return index_ != value.index_;
		}

		const T& operator*() {
			return data_[index_];
		}
	};

	iterator begin() {
		return iterator(0, data_);
	}

	iterator end() {
		return iterator(SIZE, data_);
	}

	const_iterator begin() const {
		return const_iterator(0, data_);
	}

	const_iterator end() const {
		return const_iterator(SIZE, data_);
	}
};

#endif	/* ARRAY_HPP */

