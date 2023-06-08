#ifndef SESSION_HPP
#define SESSION_HPP

#include "./room.hpp"


namespace boostjack {
class session : public player, public std::enable_shared_from_this<session> {
	boost::asio::ip::tcp::socket _socket;
	room& _room; 


	void listen() {
		auto self(shared_from_this());
		char* request = new char[max_data_size + 1] { };
		_socket.async_read_some(boost::asio::buffer(request, max_data_size), [this, self, request](const boost::system::error_code& ec, std::size_t) {
			if (!ec) {
				// ready action
				if (request[0] == 1)
				{ _room.ready(self); }
				// // get card
				// else if (false)
				// { }
				// // drop cards
				// else if (false)
				// { }

				listen();
			}
			else
			{ close(); }
		});
	}

	void close() {
		_room.leave(shared_from_this());
		_socket.shutdown(boost::asio::socket_base::shutdown_both);
		_socket.close();

		std::cerr << "[" << _nickname << " disconnected]" << std::endl;
	}

public:
	session(boost::asio::ip::tcp::socket socket, room& room, const char* nickname)
	: _socket(std::move(socket)), _room(room) {
		_nickname = strdup(nickname);
		
		std::cout << "[" << _nickname << " connected]" << std::endl;
	}

	void start_handling() {
		_room.join(shared_from_this());

		listen();
	}

	void send(const char* response) override {
		auto self(shared_from_this());
		_socket.async_write_some(boost::asio::buffer(response, max_data_size), [this, self, response](const boost::system::error_code& ec, std::size_t) {
			if (ec)
			{ close(); }
		});
	}

	~session()
	{ delete[] _nickname; }
};
}

#endif