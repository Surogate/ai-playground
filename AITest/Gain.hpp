/* 
 * File:   Gain.hpp
 * Author: Francois Ancel (francoisancel [at] gmail.com)
 * Copyright : BSD license
 * Created on September 16, 2011, 1:37 PM
 */

#ifndef GAIN_HPP
#define	GAIN_HPP

#include <map>
#include <cmath>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include "ConditionnalFunctor.hpp"

namespace ID3 {

	template < typename Attribue, typename Result >
	class Gain {
	public:
		typedef std::map< Attribue, float > AttribueMap;
		typedef std::map< Result, float > ResultMap;
		typedef std::map< Attribue, ResultMap > AttribuePerResultMap;

		Gain() : attribue_per_result_(), attribue_value_total_(), result_value_yes_(), entropy_(0), log_2_(std::log(2.f)), size_total_(0) 
		{}

		template < typename AttribueContainer, typename ResultContainer>
		Gain(const AttribueContainer& value, const ResultContainer& result)
			: attribue_per_result_(), attribue_value_total_(), result_value_yes_(), entropy_(0), log_2_(std::log(2.f)), size_total_(0) {
				getGain(value, result);
		}

		template < typename AttribueContainer, typename ResultContainer>
		Gain(const AttribueContainer& value, const ResultContainer& result, ConditionnalFunctor& func)
			: attribue_per_result_(), attribue_value_total_(), result_value_yes_(), entropy_(0), log_2_(std::log(2.f)), size_total_(0) {
				getGain(value, result, func);
		}

		template < typename AttribueContainer, typename ResultContainer>
		float operator()(const AttribueContainer& value, const ResultContainer& result) {
			return getGain(value, result);
		}

		template < typename AttribueContainer, typename ResultContainer>
		float getGain(const AttribueContainer& value, const ResultContainer& result) {
			typename AttribueContainer::const_iterator ita = value.begin();

			typename ResultContainer::const_iterator itr = result.begin();
			typename ResultContainer::const_iterator itre = result.end();

			while (itr != itre) {
				attribue_per_result_[*ita][*itr]++;
				attribue_value_total_[*ita]++;
				result_value_yes_[*itr]++;

				++itr;
				++ita;
				++size_total_;
			}

			result_ = entropy_ =  getEntropy(result_value_yes_.begin(), result_value_yes_.end(),  size_total_);
			typename AttribuePerResultMap::iterator it_yes = attribue_per_result_.begin();
			typename AttribuePerResultMap::iterator it_yes_e = attribue_per_result_.end();
			typename AttribueMap::iterator it_tot = attribue_value_total_.begin();

			while (it_yes != it_yes_e) {
				result_ -= (it_tot->second / size_total_) * getEntropy(it_yes->second.begin(), it_yes->second.end(), it_tot->second);
				++it_yes;
				++it_tot;
			}
			return result_;
		}

		template < typename AttribueContainer, typename ResultContainer, typename ConditionnalFunctor>
		float getGain(const AttribueContainer& value, const ResultContainer& result, ConditionnalFunctor& func) {
			func.reset();
			typename AttribueContainer::const_iterator ita = value.begin();

			typename ResultContainer::const_iterator itr = result.begin();
			typename ResultContainer::const_iterator itre = result.end();

			while (itr != itre) {
				if (func()) {
					attribue_per_result_[*ita][*itr]++;
					attribue_value_total_[*ita]++;
					result_value_yes_[*itr]++;

					++size_total_;
				}

				++func;
				++itr;
				++ita;
			}

			result_ = entropy_ =  getEntropy(result_value_yes_.begin(), result_value_yes_.end(),  size_total_);
			typename AttribuePerResultMap::iterator it_yes = attribue_per_result_.begin();
			typename AttribuePerResultMap::iterator it_yes_e = attribue_per_result_.end();
			typename AttribueMap::iterator it_tot = attribue_value_total_.begin();

			while (it_yes != it_yes_e) {
				result_ -= (it_tot->second / size_total_) * getEntropy(it_yes->second.begin(), it_yes->second.end(), it_tot->second);
				++it_yes;
				++it_tot;
			}
			return result_;
		}

		const float& getGlobalEntropy() const {
			return entropy_;
		}

		const float& getResult() const {
			return result_;
		}

		const AttribueMap& getAttribueMap() const {
			return attribue_value_total_;
		}

		Result getMainResult() const {
			Result answer = Result();
			float higher = 0;

			typename ResultMap::const_iterator it = result_value_yes_.begin();
			typename ResultMap::const_iterator ite = result_value_yes_.end();

			while (it != ite) {
				if (it->second > higher)
					answer = it->first;
				++it;
			}
			return answer;
		}

		float getSize() const {
			return size_total_;
		}

		unsigned int size_value() {
			return result_value_yes_.size();
		}

		template <typename ResultContainer>
		static Result getRandomResult(const ResultContainer& container) {
			boost::random::uniform_int_distribution<int> distribution(0, container.size());
			int value = distribution(boost::random::mt19937());

			typename ResultContainer::const_iterator it = container.begin();
			typename ResultContainer::const_iterator ite = container.end();

			int i = 0;
			while (it != ite && i < value)
			{
				++it;
				++i;
			}

			return *it;
		}

	private:
		AttribuePerResultMap attribue_per_result_;
		AttribueMap attribue_value_total_;
		ResultMap result_value_yes_;
		float entropy_;
		float result_;
		float log_2_;
		float size_total_;

		template <typename Iterator>
		inline float getEntropy(Iterator begin, Iterator end, float size) {
			float result = 0;

			while (begin != end) {
				float p = begin->second / size;
				result -= p * (std::log(p) / log_2_);
				++begin;
			}
			return result;
		}
	};

}
#endif	/* GAIN_HPP */

