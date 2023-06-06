#ifndef SESSION_HPP
#define SESSION_HPP

#include "./room.hpp"

#include "boost/asio.hpp"
#include <iostream>


namespace boostjack {
enum { max_data_size = 16 };

class session : public player, public std::enable_shared_from_this<session> {
	boost::asio::ip::tcp::socket _socket;
	room& _room; 


	void listen() {
		auto self(shared_from_this());
		char* request = new char[max_data_size + 1] { };
		_socket.async_read_some(boost::asio::buffer(request, max_data_size), [this, self, request](const boost::system::error_code& ec, std::size_t) {
			if (!ec) {
				if (!std::strcmp(request, "ping")) {
					_room.ping();
				}
				std::cout << ">> " << request << std::endl;

				listen();
			}
			else {
				_room.leave(shared_from_this());
				_socket.shutdown(boost::asio::socket_base::shutdown_both);
				_socket.close();
				std::cerr << "[Session closed]" << std::endl;
			}
		});
	}

public:
	session(boost::asio::ip::tcp::socket socket, room& room)
	: _socket(std::move(socket)), _room(room)
	{ std::cout << "[New session]" << std::endl; }

	void start_handling() {
		_room.join(shared_from_this());

		listen();
	}

	void send(const char* response) override {
		auto self(shared_from_this());
		_socket.async_write_some(boost::asio::buffer(response, max_data_size), [this, self, response](const boost::system::error_code& ec, std::size_t) {
			if (!ec)
			{ std::cout << "<< " << response << std::endl; }
			else {
				_room.leave(shared_from_this());
				_socket.shutdown(boost::asio::socket_base::shutdown_both);
				_socket.close();
				std::cerr << "[Session closed]" << std::endl;
			}
		});
	}
};
}

#endif