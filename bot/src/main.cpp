#include "../include/bot_client.hpp"
#include "../include/exbot.hpp"


int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cerr << "Usage: boostjack_client <host> <port> <nickname>" << std::endl;
		return -1;
	}

	boost::asio::io_context io_context;
	exbot bot;
	try {
		boostjack::bot_client client(io_context, bot);
		client.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(argv[1]), std::atoi(argv[2])), argv[3]);

		std::string input;
		while (std::cin >> input) {
			if (input == "/q") { break; }

			if (input == "ready") {
				char* request = new char[1] { 1 };
				client.send(request);
			}
		}

		client.disconnect();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}