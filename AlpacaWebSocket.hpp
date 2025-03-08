#include "AlpacaWebSocket.cpp"


// Constructor: Set initial values.
AlpacaWebSocket::AlpacaWebSocket()
    : m_url(""), m_isConnected(false)

{
    std::cout << "AlpacaWebSocket created. Ready to connect." << std::endl;
}

// Destructor: Disconnect if still connected.
AlpacaWebSocket::~AlpacaWebSocket() {
    if (m_isConnected) {
        disconnect();
    }
    std::cout << "AlpacaWebSocket destroyed." << std::endl;
}

#include "AlpacaWebSocket.cpp"
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <iostream>

// For brevity, we reuse our namespace aliases and type alias:
using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

void AlpacaWebSocket::connect(const std::string& url) {
    
   
    m_isConnected = true;

    std::cout << "Connected to " << m_url << std::endl;
}

// Disconnect method: Simulates disconnecting.
void AlpacaWebSocket::disconnect() {
    // Here you would close your WebSocket connection gracefully.
    m_isConnected = false;
    std::cout << "Disconnected from " << m_url << std::endl;
}