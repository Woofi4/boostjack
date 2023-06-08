#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./constants.hpp"


namespace boostjack {
struct player {
	const char* _nickname;
	bool is_ready;

	virtual void send(const char*) = 0;
	virtual ~player() { }
};
}

#endif