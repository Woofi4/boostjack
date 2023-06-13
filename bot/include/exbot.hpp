#ifndef BOT_HPP
#define BOT_HPP

#include "ibot.hpp"
#include <iostream>

class exbot : public ibot {
	bool should_stand = false;
	bool should_hit = false;
	unsigned round = 0;

public:
	const char* handle(void* data = nullptr) override {
		++round;

		if (should_hit) {
			should_hit = false;

			std::cout << "hit" << std::endl;
			return hit();
		}

		if (should_stand) {
			std::cout << "stand" << std::endl;
			return stand();
		}

		if (score == 15 || score == 16) {
			should_stand = true;

			std::cout << "hit" << std::endl;
			return hit();
		}

		if (score < 15) {
			return hit();
		}

		if (round >= 3 && score > 16) {
			should_hit = true;
			should_stand = true;

			std::cout << "hit" << std::endl;
			return hit();
		}

		return nullptr;
	}

	void reset() override {
		should_stand = false;
		should_hit = false;
		round = 0;
		score = 0;
	}
};


#endif