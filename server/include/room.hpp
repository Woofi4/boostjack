#ifndef ROOM_HPP
#define ROOM_HPP

#include "./player.hpp"
#include "./game.hpp"

#include <boost/asio.hpp>
#include <set>
#include <iostream>


namespace boostjack {
class room {
	std::set<std::shared_ptr<player>> _players;
	game gm;
	

	void send_to_other(std::shared_ptr<player> player, const char* alert) {
		for (auto p : _players) {
			if (p == player)
			{ continue; }
			
			// TODO: do nicely data formating
			std::size_t nickname_size = std::strlen(player->nickname) + 1;
			std::size_t alert_size = std::strlen(alert) + 1;
			char msg[nickname_size + alert_size + 1] { };
			memcpy(&msg[1], player->nickname, nickname_size - 1);
			msg[nickname_size] = ' ';
			memcpy(&msg[nickname_size + 1], alert, alert_size - 1);
			
			p->send(msg);
		}
	}

	bool ready_check() const {
		for (auto p : _players) {
			if (!p->is_ready)
			{ return false; }
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

		send_to_other(player, "left");
	}

	void ready(std::shared_ptr<player> player) {
		if (player->is_ready)
		{ return; }

		player->is_ready = true;
		send_to_other(player, "ready");

		std::cout << "[" << player->nickname << " ready]" << std::endl;

		if (ready_check() && _players.size() >= min_players)
		{ start(); }
	}

	void start() {
		gm.start(_players);

		char* response = new char[1] { 1 };
		for (auto p : _players) {	
			p->send(response);
			
			std::vector<card> cards = gm.get_card(2);
			p->score += cards[0] + cards[1];
			char* cards_response = new char[4] {2, 2, (char) cards[0], (char) cards[1] };
			p->send(cards_response);

			if (p->score == 21) {
				p->send("\0You win");
				send_to_other(p, "win");

				stop();
			}
			else if (p->score > 21) {
				p->send("\0You lose");
				send_to_other(p, "lose");
				p->is_active = false;

				if (p == *gm._active_player) {
					gm.next();
				}

				gm._players_order.erase(std::find(gm._players_order.begin(), gm._players_order.end(), p));

				if (!is_active()) {
					for (auto p : _players) {
						p->send("\0Tie");
					}
					stop();
				}
			}
		}

		if (is_active()) {
			notify();
		}

		std::cout << "[Game start]" << std::endl;
	}

	void get_card() {
		std::vector<card> cards = gm.get_card(1);
		(*gm._active_player)->score += cards[0];
		char* card_response = new char[4] {2, 1, (char) cards[0] };
		(*gm._active_player)->send(card_response);

		handle();
	}

	void handle() {
		if ((*gm._active_player)->score == 21) {
			(*gm._active_player)->send("\0You win");
			send_to_other(*gm._active_player, "win");

			gm._players_order.clear();
			stop();
		}
		else if ((*gm._active_player)->score > 21) {
			(*gm._active_player)->send("\0You lose");
			send_to_other(*gm._active_player, "lose");
			gm.surrender();

			if (!is_active()) {
				std::vector<std::shared_ptr<player>> winners;
				for (auto p : _players) {
					if (p->is_active) {
						if (winners.size() == 0)
						{ winners.push_back(p); }
						else if (winners[0]->score == p->score) {
							winners.push_back(p);
						}
						else if (winners[0]->score < p->score) {
							winners.clear();
							winners.push_back(p);
						}
					}
				}

				if (winners.size() == 0) {
					for (auto p : _players) {
						p->send("\0Tie");
					}
				}
				else {
					for (auto p : winners) {
						p->send("\0Win");
						send_to_other(p, "win");
					}
				}

				stop();
			}
		}

		if (is_active()) {
			gm.next();
			notify();
		}
	}

	void stand() {
		send_to_other(*gm._active_player, "stand");
		gm.stand();

		if (!is_active()) {
			std::vector<std::shared_ptr<player>> winners;
			for (auto p : _players) {
				if (p->is_active) {
					if (winners.size() == 0)
					{ winners.push_back(p); }
					else if (winners[0]->score == p->score) {
						winners.push_back(p);
					}
					else if (winners[0]->score < p->score) {
						winners.clear();
						winners.push_back(p);
					}
				}
			}

			for (auto p : winners) {
				p->send("\0Win");
				send_to_other(p, "win");
			}

			stop();
		}
		else
		{ notify(); }
	}

	// // TODO: handle this event
	// void surrender() {
	// 	send_to_other(*gm._active_player, "surrender");
	// 	gm.surrender();

	// 	if (!is_active()) 
	// 	{ notify(); }
	// }

	void stop() {
		for (auto p : _players) {
			p->is_ready = false;
			p->score = 0;
			p->send("\0Get ready");
		}
	}

	void notify()
	{ (*gm._active_player)->send("\0Your turn"); }

	bool is_full() const
	{ return _players.size() == max_players; }

	bool is_active() const
	{ return gm._players_order.size() != 0; }

	std::shared_ptr<player> active_player() const
	{ return *gm._active_player; }
};
}

#endif