/* 
 * File:   Gain.hpp
 * Author: fancel
 *
 * Created on September 16, 2011, 1:37 PM
 */

#ifndef GAIN_HPP
#define	GAIN_HPP

#include <map>
#include <cmath>

template < typename Attribue, typename Result >
class Gain {
public:
	typedef std::map< Attribue, float > AttribueMap;
	typedef std::map< Result, float > ResultMap;
	typedef std::map< Attribue, ResultMap > AttribuePerResultMap;

	template < typename AttribueContainer, typename ResultContainer>
	float operator()(const AttribueContainer& value, const ResultContainer& result) {
		float size_total = 0;
		typename AttribueContainer::const_iterator ita = value.begin();
		typename AttribueContainer::const_iterator itae = value.end();

		typename ResultContainer::const_iterator itr = result.begin();
		typename ResultContainer::const_iterator itre = result.end();

		while (itr != itre) {
			attribue_per_result_[*ita][*itr]++;
			attribue_value_total_[*ita]++;
			result_value_yes_[*itr]++;

			++itr;
			++ita;
			++size_total;
		}

		float entropy = entropy_ =  getEntropy(result_value_yes_.begin(), result_value_yes_.end(),  size_total);
		typename AttribuePerResultMap::iterator it_yes = attribue_per_result_.begin();
		typename AttribuePerResultMap::iterator it_yes_e = attribue_per_result_.end();
		typename AttribueMap::iterator it_tot = attribue_value_total_.begin();

		while (it_yes != it_yes_e) {
			entropy -= (it_tot->second / size_total) * getEntropy(it_yes->second.begin(), it_yes->second.end(), it_tot->second);
			++it_yes;
			++it_tot;
		}
		return entropy;
	}

	template < typename AttribueContainer, typename ResultContainer>
	float getGain(const AttribueContainer& value, const ResultContainer& result) {
		float size_total = 0;
		typename AttribueContainer::const_iterator ita = value.begin();
		typename AttribueContainer::const_iterator itae = value.end();

		typename ResultContainer::const_iterator itr = result.begin();
		typename ResultContainer::const_iterator itre = result.end();

		while (itr != itre) {
			attribue_per_result_[*ita][*itr]++;
			attribue_value_total_[*ita]++;
			result_value_yes_[*itr]++;

			++itr;
			++ita;
			++size_total;
		}

		float entropy = entropy_ =  getEntropy(result_value_yes_.begin(), result_value_yes_.end(),  size_total);
		typename AttribuePerResultMap::iterator it_yes = attribue_per_result_.begin();
		typename AttribuePerResultMap::iterator it_yes_e = attribue_per_result_.end();
		typename AttribueMap::iterator it_tot = attribue_value_total_.begin();

		while (it_yes != it_yes_e) {
			entropy -= (it_tot->second() / size_total) * getEntropy((it_yes->second()).begin(), (it_yes->second()).end(), it_tot->second());
			++it_yes;
			++it_tot;
		}
		return entropy;
	}

	float getGlobalEntropy() const {
		return entropy_;
	}


private:
	AttribuePerResultMap attribue_per_result_;
	AttribueMap attribue_value_total_;
	ResultMap result_value_yes_;
	float entropy_;

	template <typename Iterator>
	inline float getEntropy(Iterator begin, Iterator end, float size) {
		float result = 0;

		while (begin != end) {
			float p = begin->second / size;
			result += -1.f * p * (std::log(p) / std::log(2.f));
			++begin;
		}
		return result;
	}
};

#endif	/* GAIN_HPP */

