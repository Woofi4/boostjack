#ifndef BOT_CLIENT_HPP
#define BOT_CLIENT_HPP

#include "boost/asio.hpp"
#include <iostream>
#include <initializer_list>
#include "./ibot.hpp"

namespace boostjack {
enum { max_data_size = 17, max_nickname_length = 8 };

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

class bot_client {
	boost::asio::io_context& _io_context;
	boost::asio::ip::tcp::socket _socket;
	std::thread _thread;
	ibot& bot;

	void listen() {
		char* request = new char[max_data_size + 1] { };
		_socket.async_read_some(boost::asio::buffer(request, max_data_size), [this, request](const boost::system::error_code& ec, std::size_t) {
			if (!ec) {
				if (request[0] == 0) {
					std::cout << &request[1] << std::endl;
					if (strcmp(&request[1], "Your turn") == 0) {
						const char* action = bot.handle();
						if (action)
						{ send(action); }
						else {
							std::cout << "stand" << std::endl;
							send(new const char[1] { 3 });
						}
					}
					else if (strcmp(&request[1], "Get ready") == 0) {
						bot.reset();
						std::cout << "ready" << std::endl;
						send(new const char[1] { 1 });
					}
				}
				else if (request[0] == 1)
				{ std::cout << "[Game start]" << std::endl; }
				else if (request[0] == 2) {
					std::cout << "You got: ";
					for (unsigned i = 0; i < request[1]; ++i) {
						std::cout << (int) (request[i + 2]) << " ";
						bot.add_score(request[i + 2]);	
					}
					std::cout << std::endl;
				}
				
				delete[] request;
				listen();
			}
			else {
				delete[] request;
				throw std::runtime_error("Server closed connection");
			}
		});
	}

public:
	bot_client(boost::asio::io_context& io_context, ibot& b)
	: _io_context(io_context), _socket(io_context), _thread(([&io_context](){ io_context.run(); })), bot(b)
	{ }

	void connect(const boost::asio::ip::tcp::endpoint& endpoint, const char* nickname) {
		_socket.async_connect(endpoint, [this, nickname](const boost::system::error_code& ec) {
			if (!ec) {
				std::cout << "[Connected]" << std::endl;

				_socket.send(boost::asio::buffer(nickname, std::max((size_t) boostjack::max_nickname_length, std::strlen(nickname))));
				_socket.send(boost::asio::buffer(new const char[1] { 1 }, 1));
				listen();
			}
			else
			{ throw std::runtime_error("Connection error"); }
		});
	}

	void disconnect() {
		_socket.shutdown(boost::asio::socket_base::shutdown_both);
		_socket.close();
		_io_context.stop();
		_thread.join();
		std::cout << "[Disconnected]" << std::endl;
	}

	void send(const char* request) {
		if (request[0] == 0)
		{ std::cout << "<< " << &request[1] << std::endl; }
		
		_socket.async_write_some(boost::asio::buffer(request, max_data_size), [this](const boost::system::error_code& ec, std::size_t) {
			if (ec)
			{ throw std::runtime_error("Socket corrupted"); }
		});
	}
};
}

#endif