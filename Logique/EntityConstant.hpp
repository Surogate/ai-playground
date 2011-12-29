
#ifndef EntityConstant
#define EntityConstant

namespace Logique {
	enum Constant 
	{
			BASEFOODTIME = 12,
			BASEFOODDECREASE = 1,
			FOOD_MAX = 15,
			FOOD_REP_LIMIT_START = 16,
			FOOD_REP_LIMIT_END = 7,
			MOVE_TIME = 2
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
