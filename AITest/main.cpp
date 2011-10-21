
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

namespace ID3 {

	namespace TypedTree {

		template <>
		struct Debugger<Wind> {
			Debugger() { std::cout << "Wind" << std::endl; }
			Debugger(const Wind& value) { std::cout << "Wind value " << getValue(value) << std::endl; }
			const char* getValue(const Wind& value) {
				if (value == Weak)
					return "Weak";
				return "Strong";
			}
		};

		template <>
		struct Debugger<Humidity> {
			Debugger() { std::cout << "Humidity" << std::endl; }
			Debugger(const Humidity& value) { std::cout << "Humidity value " << getValue(value) << std::endl; }
			const char* getValue(const Humidity& value) {
				if (value == High)
					return "High";
				return "Normal";
			}
		};

		template <>
		struct Debugger<Temperature> {
			Debugger() { std::cout << "Temperature" << std::endl; }
			Debugger(const Temperature& value) { std::cout << "Temperature value " << getValue(value) << std::endl; }
			const char* getValue(const Temperature& value) {
				if (value == Hot)
					return "Hot";
				if (value == Mild)
					return "Mild";
				return "Cool";
			}
		};

		template <>
		struct Debugger<Outlook> {
			Debugger() { std::cout << "Outlook" << std::endl; }
			Debugger(const Outlook& value) { std::cout << "Outlook value " << getValue(value) << std::endl;}
			const char* getValue(const Outlook& value) {
				if (value == Sunny)
					return "Sunny";
				if (value == Overcast)
					return "Overcast";
				return "Rain";
			}
		};

		template <>
		struct Debugger<PlayBall> {
			Debugger() { std::cout << "PlayBall" << std::endl; }
			Debugger(const PlayBall& value) { std::cout << "PlayBall value " << getValue(value) << std::endl;}
			const char* getValue(const PlayBall& value) {
				if (value == Yes)
					return "Yes";
				return "No";
			}
		};

	}

}

typedef ID3::ID3Class5<Outlook, Temperature, Humidity, Wind, PlayBall> ID3Test5;
typedef ID3Test5::Answer Answer;

typedef ID3::ID3Class4<Outlook, Temperature, Humidity, PlayBall> ID3Test4;
typedef ID3Test4::Answer Answer4;

typedef ID3::ID3Class3<Outlook, Temperature, PlayBall> ID3Test3;
typedef ID3Test3::Answer Answer3;

typedef ID3::ID3Class2<Outlook, PlayBall> ID3Test2;
typedef ID3Test2::Answer Answer2;

void initID3(ID3Test5& id3) {
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
	ID3Test5 id3;
	ID3Test4 id34;
	ID3Test3 id33;
	ID3Test2 id32;

	initID3(id3);

	id3.generateTree();
	id34.generateTree();
	id33.generateTree();
	id32.generateTree();

	Answer value = id3.decide(Sunny, Hot, Normal, Weak);
	ID3::TypedTree::Debugger<PlayBall> answerDebug(*value);
	value = id3.decide(Overcast, Hot, High, Weak);
	ID3::TypedTree::Debugger<PlayBall> answerDebug1(*value);
	value = id3.decide(Rain, Hot, High, Weak);
	ID3::TypedTree::Debugger<PlayBall> answerDebug2(*value);
	value = id3.decide(Sunny, Hot, High, Weak);
	ID3::TypedTree::Debugger<PlayBall> answerDebug3(*value);
	value = id3.decide(Rain, Hot, High, Strong);
	ID3::TypedTree::Debugger<PlayBall> answerDebug4(*value);

	std::cin.get();
	return 0;
}