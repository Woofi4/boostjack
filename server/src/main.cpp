#include "../include/server.hpp"


int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: boostjack_server <port>" << std::endl;
		return -1;
	}

	boost::asio::io_context io_context;
	try {
		boostjack::server server(io_context, std::atoi(argv[1]));
		server.run();

		std::string input;
		while (std::cin >> input)
		{ if (input == "/sd") { break; } }

		server.stop();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}