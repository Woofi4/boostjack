#ifndef GAME_HPP
#define GAME_HPP

#include "./player.hpp"
#include "./constants.hpp"
#include <algorithm>
#include <deque>
#include <iostream>
#include <boost/asio.hpp>
#include <list>
#include <set>
#include <random>


namespace boostjack {
class game {
	std::deque<card> deck;
	std::list<std::shared_ptr<player>> _players_order;
	std::list<std::shared_ptr<player>>::iterator _active_player;
	std::random_device rd { };
	std::default_random_engine re { rd() };
	
public:
	void start(std::set<std::shared_ptr<player>> players) {
		for (auto p : players) {
			_players_order.push_back(p);
			p->is_active = true;
		}

		deck = deck36;
		std::shuffle(deck.begin(), deck.end(), re);
		_active_player = _players_order.begin();
	}

	void next() {
		++_active_player;
	
		if (_active_player == _players_order.end())
		{ _active_player = _players_order.begin(); }

		if (!(*_active_player)->is_active)
		{ next(); }
	}

	void surrender() {
		if (_players_order.size() == 1) {
			(*_active_player)->is_active = false;
			_players_order.erase(_active_player);
		}
		else {
			std::list<std::shared_ptr<player>>::iterator player = _active_player;
			next();
			_players_order.erase(player);
			(*player)->is_active = false;
		}
	}

	std::vector<card> get_card(std::size_t count) {
		std::vector<card> hit;
		for (std::size_t i = 0; i < count; ++i) {
			card crd = deck.back();
			hit.push_back(crd);
			deck.pop_back();
		}

		return hit;
	}

	void stand() {
		if (_players_order.size() == 1) {
			_players_order.erase(_active_player);
		}
		else {
			std::list<std::shared_ptr<player>>::iterator player = _active_player;
			next();
			_players_order.erase(player);
		}
	}

	friend class room;
};
}

#endif