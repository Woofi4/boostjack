#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace boostjack {
struct player {
	virtual void send(const char*) = 0;
	virtual ~player() { }
};
}

#endif