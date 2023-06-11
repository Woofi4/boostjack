#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./constants.hpp"
#include <cstring>

namespace boostjack {
struct player {
	const char* nickname;
	bool is_ready;

	std::size_t score;
	bool is_active;


	player(const char* name)
	: nickname(::strdup(name)), is_ready(false), score(0), is_active(false)
	{ }

	~player()
	{ delete[] nickname; }


	virtual void send(const char*) = 0;
};
}

#endif