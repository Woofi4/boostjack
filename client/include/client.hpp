#include "boost/asio.hpp"
#include <iostream>


namespace boostjack {
enum { max_data_size = 16 };

class client {
	boost::asio::io_context& _io_context;
	boost::asio::ip::tcp::socket _socket;
	std::thread _thread;


	void listen() {
		char* request = new char[max_data_size + 1] { };
		_socket.async_read_some(boost::asio::buffer(request, max_data_size), [this, request](const boost::system::error_code& ec, std::size_t) {
			if (!ec) { 
				std::cout << ">> " << request << std::endl;
				listen();
			}
			else
			{ throw std::runtime_error("Server closed"); }
		});
	}

public:
	client(boost::asio::io_context& io_context)
	: _io_context(io_context), _socket(io_context), _thread(([&io_context](){ io_context.run(); }))
	{ }

	void connect(const boost::asio::ip::tcp::endpoint& endpoint) {
		_socket.async_connect(endpoint, [this](const boost::system::error_code& ec) {
			if (!ec) {
				std::cout << "[Connected]" << std::endl;
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
		std::cout << "<< " << request << std::endl;
		_socket.async_write_some(boost::asio::buffer(request, max_data_size), [this](const boost::system::error_code& ec, std::size_t) {
			if (ec)
			{ throw std::runtime_error("Socket corrupted"); }
		});
	}
};
}