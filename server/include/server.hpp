#ifndef SERVER_HPP
#define SERVER_HPP

#include "./session.hpp"


namespace boostjack {
class server {
	boost::asio::io_context& _io_context;
	boost::asio::ip::tcp::acceptor _acceptor;
	std::thread _thread;
	room _room;


	void accept() {
		_acceptor.async_accept([this](const boost::system::error_code& ec, boost::asio::ip::tcp::socket socket) {
			if (!ec) {
				std::make_shared<session>(std::move(socket), _room)->start_handling();
			}
			else {
				socket.shutdown(boost::asio::socket_base::shutdown_both);
				socket.close();
			}

			accept();
		});
	}

public:
	server(boost::asio::io_context& io_context, int port)
	: _io_context(io_context),_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _thread(([&io_context](){ io_context.run(); }))
	{ }

	void run() {
		std::cout << "[Server started]" << std::endl;
		accept();
	}

	void stop() {
		_acceptor.close();
		_io_context.stop();
		_thread.join();
		std::cout << "[Server stopped]" << std::endl;
	}
};
}

#endif