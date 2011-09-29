
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

	PlayBall value = id3.decide(Sunny, Hot, Normal, Weak);
	if (value == Yes)
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;

	return 0;
}