#pragma once

#include <bits/stdc++.h>
#include <uWS/uWS.h>

class Glotter {

public:

    Glotter(){
        uws.onHttpRequest([&](uWS::HttpResponse* res, uWS::HttpRequest req, char* data, size_t len, size_t remain){
            std::clog << "'" << req.getUrl().toString() << "'" << std::endl;
            if (req.getMethod() == uWS::METHOD_GET) {
                if (req.getUrl().toString() == "/") {
                    auto html = readFile("index.html");
                    res->end(html.c_str(), html.size());
                } else if (req.getUrl().toString() == "/style.css") {
                    auto css = readFile("style.css");
                    res->end(css.c_str(), css.size());
                } else if (req.getUrl().toString() == "/glotter.js") {
                    auto js = readFile("glotter.js");
                    res->end(js.c_str(), js.size());
                }
            }
        });
        uws.onConnection([&](uWS::WebSocket<uWS::SERVER>* conn, auto){
            std::cout << "something connected!" << std::endl;
            ws = conn;
        });
        uws.listen(57077);
        std::thread([&]{uws.run();}).detach();
        std::clog << "hoooray!" << std::endl;
    }

    void addEdge(int a, int b){
        send("addEdge " + std::to_string(a) + " " + std::to_string(b));
    }
    void resize(int n){
        send("resize " + std::to_string(n));
    }

private:

    static std::string readFile(const std::string& path) {
        std::ifstream file;
        file.exceptions(std::ios::badbit | std::ios::failbit);
        file.open(path);
        std::string buf;
        std::getline(file, buf, '\0');
        return buf;
    }
    void send(const std::string& s) {
        if (!ws) {
            std::cout << "Waiting for connection..." << std::endl;
            while (!ws)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        ws->send(s.c_str(), s.size(), uWS::OpCode::TEXT);
    }

    uWS::Hub uws;
    uWS::WebSocket<uWS::SERVER>* ws = nullptr;

};