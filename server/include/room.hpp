#ifndef ROOM_HPP
#define ROOM_HPP

#include "./player.hpp"

#include <boost/asio.hpp>
#include <set>


namespace boostjack {
class room {
	std::set<std::shared_ptr<player>> _players;

public:
	void join(std::shared_ptr<player> player) {
		_players.insert(player);
	}

	void leave(std::shared_ptr<player> player) {
		_players.erase(player);
	}

	void ping() {
		for (auto player : _players) {
			player->send("ping");
		}
	}
};
}

#endif