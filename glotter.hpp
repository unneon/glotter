#pragma once

#include <bits/stdc++.h>
#include <seasocks/PrintfLogger.h>
#include <seasocks/Server.h>
#include <seasocks/WebSocket.h>

class Glotter {

public:

    Glotter():
        server(std::make_shared<seasocks::PrintfLogger>(seasocks::Logger::Level::INFO))
    {
        server.addWebSocketHandler("/ws", std::make_shared<HandlerWS>(this));
        server.serve(".", 57077);
    }

private:

    struct HandlerWS : public seasocks::WebSocket::Handler {

        Glotter* glotter;
        HandlerWS(Glotter* glotter):glotter(glotter){}

        void onConnect(seasocks::WebSocket* evconn) override {
            if (!glotter->conn) glotter->conn = evconn;
            // TODO: handle error
        }
        void onDisconnect(seasocks::WebSocket*) override {
            // TODO: handle disconnect
        }
        void onData(seasocks::WebSocket*, const char*) override {
            // TODO: handle data
        }

    };
    friend HandlerWS;

    seasocks::Server server;
    seasocks::WebSocket* conn = nullptr;

};