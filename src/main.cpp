#include "../include/AlpacaWebSocket.hpp"

int main() {
    AlpacaWebSocket ws;
    ws.connect("wss://stream.data.alpaca.markets/v2/sip");
    // You can add more logic here...
   
    return 0;
}