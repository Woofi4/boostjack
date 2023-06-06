#include "../include/client.hpp"


int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: boostjack_client <host> <port>" << std::endl;
		return -1;
	}

	boost::asio::io_context io_context;
	try {
		boostjack::client client(io_context);
		client.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(argv[1]), std::atoi(argv[2])));

		std::string input;
		while (std::cin >> input) {
			if (input == "/q") { break; }

			client.send(input.c_str());
		}

		client.disconnect();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}