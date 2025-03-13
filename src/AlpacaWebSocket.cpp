#include "../include/AlpacaWebSocket.hpp"
#include <boost/beast/ssl.hpp>                // For SSL-related functionality in Beast.
#include <boost/beast/websocket/ssl.hpp>        // For WebSocket over SSL support.
#include <boost/asio/ssl.hpp>                   // For Boost.Asio SSL support.
#include <boost/asio/connect.hpp>               // For connection helper functions.
#include <boost/beast/core.hpp>                 // Core functionalities for Boost.Beast.
#include <memory>                               // For std::unique_ptr.

using tcp = asio::ip::tcp;                      // Alias for TCP using Boost.Asio.
namespace ssl = asio::ssl;                      // Alias for SSL from Boost.Asio.
namespace websocket = boost::beast::websocket;  // Alias for WebSocket functionalities from Beast.


// Constructor: Initializes member variables.
AlpacaWebSocket::AlpacaWebSocket()
    : m_url(""), m_isConnected(false)
{
    std::cout << "AlpacaWebSocket created. Ready to connect." << std::endl;
}


// Destructor: If still connected, disconnect gracefully.
AlpacaWebSocket::~AlpacaWebSocket() {
    if (m_isConnected) {
        disconnect();
    }
    std::cout << "AlpacaWebSocket destroyed." << std::endl;
}


// Connect to the WebSocket endpoint specified by 'url'.
void AlpacaWebSocket::connect(const std::string& url) {
    // If already connected, simply report and exit.
    if (m_isConnected) {
        std::cout << "Already connected to " << m_url << std::endl;
        return;
    }

    // Store the URL.
    m_url = url;
    
    // Hardcoded values for the Alpaca paper API stream.
    std::string host = "paper-api.alpaca.markets";
    std::string port = "443";      // Port for secure (wss) connections.
    std::string target = "/stream"; // The API endpoint for streaming.

    try {
        // Create an I/O context to handle asynchronous I/O operations.
        asio::io_context io_context;
        
        // Resolve the host name and port to one or more endpoints.
        tcp::resolver resolver(io_context);
        auto const results = resolver.resolve(host, port);
        
        // Create an SSL context configured for a TLS v1.2 client.
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_verify_mode(ssl::verify_none); // Disable certificate verification (for testing only).
        
        // Allocate and store the WebSocket stream as a member variable.
        // Note: m_ws is a unique_ptr to a WebSocket stream over an SSL stream wrapping a TCP socket.
        m_ws = std::make_unique<websocket::stream<ssl::stream<tcp::socket>>>(io_context, ctx);
        
        // Establish a TCP connection to one of the resolved endpoints.
        asio::connect(boost::beast::get_lowest_layer(*m_ws), results);
        
        // Set the SNI hostname (Server Name Indication) for SSL. This is required by many servers.
        if (!SSL_set_tlsext_host_name(m_ws->next_layer().native_handle(), host.c_str())) {
            boost::system::error_code ec{
                static_cast<int>(::ERR_get_error()),
                asio::error::get_ssl_category()
            };
            throw boost::system::system_error(ec);
        }
        
        // Perform the SSL handshake with the server.
        m_ws->next_layer().handshake(ssl::stream_base::client);
        
        // Perform the WebSocket handshake. This upgrades the TCP/SSL connection to a WebSocket connection.
        m_ws->handshake(host, target);
        
        // Mark the connection as established.
        m_isConnected = true;
        std::cout << "Connected to " << m_url << std::endl;
        
        // At this point, you would normally enter a loop to read/write messages
        // or hand off the WebSocket stream to another part of your application.
    

    } catch (const std::exception& e) {
        // Print any errors encountered during connection and mark connection as failed.
        std::cerr << "Error connecting to " << m_url << ": " << e.what() << std::endl;
        m_isConnected = false;
    }
}


//Constantly read from Websocket Stream
// void AlpacaWebSocket::read() {
     
// }


// Disconnect from the WebSocket gracefully.
void AlpacaWebSocket::disconnect() {
    if (m_ws && m_isConnected) {
        try {
            // Send a WebSocket close frame with a normal closure code.
            m_ws->close(websocket::close_code::normal);
            std::cout << "WebSocket closed gracefully." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error during disconnect: " << e.what() << std::endl;
        }
    }
    // Update connection status and release the WebSocket stream.
    m_isConnected = false;
    m_ws.reset();
    std::cout << "Disconnected from " << m_url << std::endl;
}