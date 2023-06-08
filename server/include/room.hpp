#ifndef ROOM_HPP
#define ROOM_HPP

#include "./player.hpp"

#include <boost/asio.hpp>
#include <set>
#include <iostream>


namespace boostjack {
class room {
	std::set<std::shared_ptr<player>> _players;

	void send_to_other(std::shared_ptr<player> player, const char* alert) {
		for (auto pl : _players) {
			if (pl == player)
			{ continue; }
			
			// TODO: do nicely data formating
			std::size_t nickname_size = std::strlen(player->_nickname) + 1;
			std::size_t alert_size = std::strlen(alert) + 1;
			char msg[nickname_size + alert_size + 1] { };
			memcpy(&msg[1], player->_nickname, nickname_size - 1);
			msg[nickname_size] = ' ';
			memcpy(&msg[nickname_size + 1], alert, alert_size - 1);
			
			pl->send(msg);
		}
	}

	bool ready_check() {
		for (auto pl : _players) {
			if (!pl->is_ready) {
				return false;
			}
		}

		return true;
	}

public:
	void join(std::shared_ptr<player> player) {
		_players.insert(player);

		send_to_other(player, "join");
	}

	void leave(std::shared_ptr<player> player) {
		_players.erase(player);
		player->is_ready = false;

		send_to_other(player, "leave");
	}

	void ready(std::shared_ptr<player> player) {
		if (player->is_ready)
		{ return; }

		player->is_ready = true;
		send_to_other(player, "ready");

		std::cout << "[" << player->_nickname << " ready]" << std::endl;

		if (ready_check() && _players.size() >= boostjack::min_players) {
			for (auto pl : _players) {
				pl->send("\0Game start");
			}

			std::cout << "[Game start]";
		}
	}
};
}

#endif