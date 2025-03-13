#ifndef ALPACAWEBSOCKET_HPP
#define ALPACAWEBSOCKET_HPP
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>

// Namespace aliases
namespace beast = boost::beast;         // Boost.Beast
namespace websocket = beast::websocket;   // WebSocket functionality
namespace asio = boost::asio;             // Boost.Asio
using tcp = asio::ip::tcp;                // TCP

class AlpacaWebSocket {
public:
    // Constructor
    AlpacaWebSocket();
    // Destructor
    ~AlpacaWebSocket();
    // Connect to a WebSocket endpoint
    void connect(const std::string& url);
    // Disconnect from the WebSocket
    void disconnect();

    // Read from the WebSocket stream
    void read();

    // m_ws holds the secure WebSocket stream used for communication.
    std::unique_ptr<boost::beast::websocket::stream<boost::asio::ssl::stream<tcp::socket>>> m_ws;
private:
    // Private member variables
    std::string m_url;
    bool m_isConnected;
    
};


#endif // ALPACAWEBSOCKET_HPP