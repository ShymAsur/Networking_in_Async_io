#pragma once
#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"
//each time a client is connected a connection is created.
//acceot connection is a task too.
namespace olc {
	namespace net {
		template <typename T>
		class server_interface {
		public:
			server_interface(uint16_t port) :
				m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
			{

			}
			virtual ~server_interface() {
				Stop();
			}
			bool Start() 
			{
				try {
					WaitForClientConnection();
					m_threadContext = std::thread([this]() {m_asioContext.run(); });
				}
				catch (std::exception &e) {
					std::cerr << "[SERVER] Exception: " << e.what()<< std::endl;
					return false;
				}
				std::cout << "Server Started !" << std::endl;
			}
			void Stop() 
			{
				m_asioContext.stop();
				if (m_threadContext.joinable()) m_threadContext.join();
				std::cout << "Server Stopped !\n";
			}
			//ASYNC -->work done by asio.
			void WaitForClientConnection() {
				m_asioAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
					if (!ec) {

						std::cout << "Server New Connection: " << socket.remote_endpoint() << "\n";
						std::shared_ptr<connection<T>> newconn =
							std::make_shared<connection<T>>(connection<T>::owner::server, m_asioContext, std::move(socket), m_qMessagesIn);
						if (OnClientConnection(newconn)) {
							m_deqConnections.push_back(std::move(newconn));
							m_deqConnections.back()->ConnectToClient(nIDCounter++);
							std::cout << "Connection's  ID [" << m_deqConnections.back()->GetID() << "]\n";
						}
						else {
							std::cout << "This Client was rejected " << "\n";
						}
					}
					else {
						std::cout << "Server Connection" << ec.message() << "\n";
					}
					WaitForClientConnection();
				})
			}
			void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg) {

			}
			void MessageAllClient(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr) {

			}
		protected:
			virtual bool OnClientConnect(std::shared_ptr<connection<T>> client) {
				return false;
			}
			virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client) {

			}
			virtual void OnMessage(std::shared_ptr<connection<T>>,message<T>&msg) {

			}
			tsqueue<owned_message<T>> m_qMessagesIn;
			std::deque<std::shared_ptr<connection<T>>> m_deqConnections;
			asio::io_context m_asioContext;
			std::thread m_threadContext;
			
			asio::ip::tcp::acceptor m_asioAcceptor;

			uint32_t nIDCounter = 10000;
		};
	}
	
}