#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE //to not include boost
#include <iostream>
#include <vector>
#include <asio.hpp>
#include <asio/ts/buffer.hpp> //memory movement 
#include <asio/ts/internet.hpp> //internet
#include <chrono>


std::vector<char> vBuffer(20*1024);

void Grabsomedata(asio::ip::tcp::socket& socket) {
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), [&](std::error_code ec, std::size_t length) {
			if (!ec) {
				std::cout << "\n\n Read " << length << " bytes\n\n";
				for (int i = 0; i < length; i++) {
					std::cout << vBuffer[i];
				}
				Grabsomedata(socket);
			}
	});
}

int main() {
	asio::error_code ec; //to handle errors

	asio::io_context context; //unique instance of asio that is platform specific.
	//now we dont want the context to exit so give it some idle work

	auto work_guard = asio::make_work_guard(context);
	//run the context in another thread
	std::thread thrContext = std::thread([&]() {context.run(); });

	//but it might run out before the read is happrning.

	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80); //get the addresss

	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint,ec);
	if (!ec) {
		std::cout << "Connected !" << std::endl;
	}
	else {
		std::cout << "Not Connected!" << ec.message()<< std::endl;
	}
	if (socket.is_open()) {
		Grabsomedata(socket);
		std::string R =
			"GET /index.html HTTP/1.1\r\n"
			"Host: david-barr.co.uk\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(R.data(), R.size()), ec); //buffer is just a container. to send data

		//waiting is generally a bad thing.
			//socket.wait(socket.wait_read); //we are waiting some time and just checking each time if there is data to be read thus why we get different number of bits each time.

			//size_t bytes = socket.available();
			//std::cout << "Bytes Available " << bytes << std::endl;
			//if (bytes > 0) {
			//	socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec); //to read data
			//	for (auto c : vBuffer) {
			//		std::cout << c;
			//	}
			//}
		

		//using grabsome
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);
		context.stop();
		if (thrContext.joinable()) thrContext.join();
	}
	return 0;
}