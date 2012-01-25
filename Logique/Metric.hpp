
#ifndef METRIC_HPP
#define METRIC_HPP

struct Metric {
	unsigned int sheepNum;
	unsigned int wolfNum;
	double sheepMoy;
	double wolfMoy;
	unsigned int sheepActionNum;
	unsigned int sheepActionNeural;
	unsigned int wolfActionNum;
	unsigned int wolfActionNeural;

	Metric(	unsigned int sheepNum_ = 0, unsigned int wolfNum_ = 0,
			double sheepMoy_ = 0, double wolfMoy_ = 0,
			unsigned int sheepActionNum_ = 0, unsigned int sheepActionNeural_ = 0,
			unsigned int wolfActionNum_ = 0, unsigned int wolfActionNeural_ = 0) 
				: sheepNum (sheepNum_), wolfNum(wolfNum_), sheepMoy(sheepMoy_), wolfMoy(wolfMoy_), 
				sheepActionNum(sheepActionNum_) , sheepActionNeural(sheepActionNeural_), 
				wolfActionNum(wolfActionNum_), wolfActionNeural(wolfActionNeural_)
	{}
};

#endif