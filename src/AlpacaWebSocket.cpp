#include "../include/AlpacaWebSocket.hpp"
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <iostream>


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




// For brevity, we reuse our namespace aliases and type alias:
using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

void AlpacaWebSocket::connect(const std::string& url) {
    // If already connected, do nothing.
    if (m_isConnected) {
        std::cout << "Already connected to " << m_url << std::endl;
        return;
    }

    // Save the URL.
    m_url = url;
    
    // Hardcode the components for Alpaca’s paper API stream.
    std::string host = "paper-api.alpaca.markets";
    std::string port = "443";      // Standard port for secure (wss) connections.
    std::string target = "/stream"; // The path for the Alpaca streaming endpoint.

    while (m_isConnected){
        try {
            // Create an I/O context.
            asio::io_context io_context;
            
            // Resolve the host.
            tcp::resolver resolver(io_context);
            auto const results = resolver.resolve(host, port);
            
            // Create an SSL context for TLS.
            asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);
            ctx.set_verify_mode(boost::asio::ssl::verify_none); // For testing only!
            
            // Create a secure WebSocket stream.
            websocket::stream<boost::beast::ssl_stream<tcp::socket>> ws(io_context, ctx);
            
            // Connect the TCP socket.
            asio::connect(boost::beast::get_lowest_layer(ws), results);
            
            // Set SNI Hostname.
            if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str())) {
                boost::system::error_code ec{
                    static_cast<int>(::ERR_get_error()),
                    boost::asio::error::get_ssl_category()
                };
                throw boost::system::system_error(ec);
            }
            
            // Perform SSL handshake.
            ws.next_layer().handshake(boost::asio::ssl::stream_base::client);
            
            // Perform the WebSocket handshake.
            ws.handshake(host, target);
            
            // Mark connection as established.
            m_isConnected = true;
            std::cout << "Connected to " << m_url << std::endl;
            
            // Read a message from the WebSocket.
            boost::beast::flat_buffer buffer;
            ws.read(buffer);
            
            // Convert the received data to a string and print it.
            std::string message = boost::beast::buffers_to_string(buffer.data());
            std::cout << "Received message: " << message << std::endl;
            
            // IMPORTANT: In a full application, you would store 'ws' as a member variable
            // and continue processing messages in a loop.
            
        } catch (const std::exception& e) {
        std::cerr << "Error connecting to " << m_url << ": " << e.what() << std::endl;
        m_isConnected = false;
        }
    }
}

// Disconnect method: Simulates disconnecting.
void AlpacaWebSocket::disconnect() {
    // Here you would close your WebSocket connection gracefully.
    m_isConnected = false;
    std::cout << "Disconnected from " << m_url << std::endl;
}