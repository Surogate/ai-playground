
#ifndef EntityConstant
#define EntityConstant

namespace Logique {
	enum Constant 
	{
		BASEFOODTIME = 10,
		BASEFOODDECREASE = 1,
		FOOD_START = 15,
		FOOD_MAX = 25,
		FOOD_REP_LIMIT_START = 17,
		FOOD_REP_LIMIT_END = 10,
		MOVE_TIME = 2,
		WAIT_TIME = 1
	};

	enum EntityAction 
	{
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		EAT,
		REPRODUCE,
		WAIT,
		ACTION_CONTAINER_SIZE
	};
}

#endif
