#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <initializer_list>


namespace boostjack {
	enum { max_data_size = 17, max_nickname_length = 8, max_players = 4, min_players = 2 };

	enum card {
		ace = 11,
		ten = 10,
		nine = 9,
		eight = 8,
		seven = 7,
		six = 6,
		king = 4,
		queen = 3,
		jack = 2,
	};

	std::initializer_list<card> deck36 = {
		ace, ace, ace, ace,
		ten, ten, ten, ten,
		nine, nine, nine, nine,
		eight, eight, eight, eight,
		seven, seven, seven, seven,
		six, six, six, six,
		king, king, king, king,
		queen, queen, queen, queen,
		jack, jack, jack, jack
	};
}

#endif