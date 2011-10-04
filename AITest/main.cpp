
#include <iostream>
#include <list>
#include <vector>
#include <functional>

#include "Array.hpp"
#include "Gain.hpp"
#include "ID3Class.hpp"

enum Wind {
	Weak,
	Strong
};

enum Humidity {
	High,
	Normal,
};

enum Temperature {
	Hot,
	Mild,
	Cool
};

enum Outlook {
	Sunny,
	Overcast,
	Rain
};

enum PlayBall {
	No,
	Yes
};

template <>
struct TypedTree::Debugger<Wind> {
	Debugger() { std::cout << "Wind" << std::endl; }
	Debugger(const Wind& value) { std::cout << "Wind value " << static_cast<int>(value) << std::endl;}
};

template <>
struct TypedTree::Debugger<Humidity> {
	Debugger() { std::cout << "Humidity" << std::endl; }
	Debugger(const Humidity& value) { std::cout << "Humidity value " << static_cast<int>(value) << std::endl;}
};

template <>
struct TypedTree::Debugger<Temperature> {
	Debugger() { std::cout << "Temperature" << std::endl; }
	Debugger(const Temperature& value) { std::cout << "Temperature value " << static_cast<int>(value) << std::endl;}
};

template <>
struct TypedTree::Debugger<Outlook> {
	Debugger() { std::cout << "Outlook" << std::endl; }
	Debugger(const Outlook& value) { std::cout << "Outlook value " << static_cast<int>(value) << std::endl;}
};


template <>
struct TypedTree::Debugger<PlayBall> {
	Debugger() { std::cout << "PlayBall" << std::endl; }
	Debugger(const PlayBall& value) { std::cout << "PlayBall value " << static_cast<int>(value) << std::endl;}
};



void initID3(ID3Class5<Outlook, Temperature, Humidity, Wind, PlayBall>& id3) {
	id3.addData(Sunny	, Hot	, High	, Weak	, No	);
	id3.addData(Sunny	, Hot	, High	, Strong, No	);
	id3.addData(Overcast, Hot	, High	, Weak	, Yes	);
	id3.addData(Rain	, Mild	, High	, Weak	, Yes	);
	id3.addData(Rain	, Cool	, Normal, Weak	, Yes	);
	id3.addData(Rain	, Cool	, Normal, Strong, No	);
	id3.addData(Overcast, Cool	, Normal, Strong, Yes	);
	id3.addData(Sunny	, Mild	, High	, Weak	, No	);
	id3.addData(Sunny	, Cool	, Normal, Weak	, Yes	);
	id3.addData(Rain	, Mild	, Normal, Weak	, Yes	);
	id3.addData(Sunny	, Mild	, Normal, Strong, Yes	);
	id3.addData(Overcast, Mild	, High	, Strong, Yes	);
	id3.addData(Overcast, Hot	, Normal, Weak	, Yes	);
	id3.addData(Rain	, Mild	, High	, Strong, No	);
}

int main(void) {
	ID3Class5<Outlook, Temperature, Humidity, Wind, PlayBall> id3;

	initID3(id3);

	id3.generateTree();

	PlayBall value = id3.decideDebug(Sunny, Hot, Normal, Weak);
	if (value == Yes)
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;

	std::cin.get();
	return 0;
}