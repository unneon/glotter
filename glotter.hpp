#pragma once

#include <bits/stdc++.h>
#include <uWS/uWS.h>

class Glotter {

public:

    Glotter(){
        uws.onHttpRequest([&](uWS::HttpResponse* res, uWS::HttpRequest req, char* data, size_t len, size_t remain){
            std::cout << (req.getMethod() == uWS::METHOD_GET ? "GET " : "(unknown method) ") << req.getUrl().toString() << std::endl;
            if (req.getMethod() == uWS::METHOD_GET) {
                auto sharefile = [&](const char* alias, const char* name){
                    if (req.getUrl().toString() == alias) {
                        auto r = readFile(name);
                        res->end(r.c_str(), r.size());
                    }
                };
                sharefile("/", "index.html");
                sharefile("/style.css", "style.css");
                sharefile("/glotter.js", "glotter.js");
                sharefile("/sigma.js", "sigma/sigma.min.js");
            }
        });
        uws.onConnection([&](uWS::WebSocket<uWS::SERVER>* conn, auto){
            std::cout << "Connected on WebSocket" << std::endl;
            ws = conn;
        });
		auto port = 57077;
        uws.listen(port);
        std::thread([&]{uws.run();}).detach();
		std::cout << "Listening on port " << port << std::endl;
    }

    void addEdge(int a, int b){
        std::cout << "Adding edge " << (a+1) << "-" << (b+1) << std::endl;
        send("addEdge " + std::to_string(a) + " " + std::to_string(b));
    }
    void setEdgeColor(int a, int b, const std::string& color) {
        std::cout << "Coloring edge " << (a+1) << "-" << (b+1) << " to " << color << std::endl;
        send("setEdgeColor " + std::to_string(a) + " " + std::to_string(b) + " " + color);
    }
    void setVertexColor(int v, const std::string& color) {
        std::cout << "Coloring vertex " << v << " to " << color << std::endl;
        send("setVertexColor " + std::to_string(v) + " " + color);
    }
    void resize(int n){
        std::cout << "Graph now hold " << n << " vertices" << std::endl;
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
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    uWS::Hub uws;
    uWS::WebSocket<uWS::SERVER>* ws = nullptr;

};
