#pragma once

#include <bits/stdc++.h>
#include <uWS/uWS.h>

class Glotter {

public:

    Glotter(){
        uws.onHttpRequest([&](uWS::HttpResponse* res, uWS::HttpRequest req, char* /*data*/, size_t /*len*/, size_t /*remain*/){
            std::cout << (req.getMethod() == uWS::METHOD_GET ? "GET " : "(unknown method) ") << req.getUrl().toString() << std::endl;
            if (req.getMethod() == uWS::METHOD_GET) {
                auto sharefile = [&](const char* alias, const char* name){
                    if (req.getUrl().toString() == alias) {
                        auto r = readFile(name);
                        res->end(r.c_str(), r.size());
                    }
                };
                sharefile("/", "web/index.html");
                sharefile("/style.css", "web/style.css");
                sharefile("/glotter.js", "web/glotter.js");
				sharefile("/bebas.ttf", "web/BEBAS___.ttf");
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

    void addEdge1(int a, int b){
        send("addEdge1 " + std::to_string(a) + " " + std::to_string(b));
    }
    void addEdge2(int a, int b){
        send("addEdge2 " + std::to_string(a) + " " + std::to_string(b));
    }
    void setEdge1Color(int a, int b, const std::string& color) {
        send("setEdge1Color " + std::to_string(a) + " " + std::to_string(b) + " " + color);
    }
	void setEdge2Color(int a, int b, const std::string& color) {
        send("setEdge2Color " + std::to_string(a) + " " + std::to_string(b) + " " + color);
	}
    void setVertexColor(int v, const std::string& color) {
        send("setVertexColor " + std::to_string(v) + " " + color);
    }
    void resize(int n){
        send("resize " + std::to_string(n));
    }

private:

    static std::string readFile(const std::string& path) {
        std::ifstream file;
        file.exceptions(std::ios::badbit | std::ios::failbit);
        file.open(path, std::ios::binary);
        std::stringstream buf;
        buf << file.rdbuf();
		std::clog << buf.str() << std::endl;
        return buf.str();
    }
    void send(const std::string& s) {
        if (!ws) {
            std::cout << "Waiting for connection..." << std::endl;
            while (!ws)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
		std::clog << s << std::endl;
        ws->send(s.c_str(), s.size(), uWS::OpCode::TEXT);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    uWS::Hub uws;
    uWS::WebSocket<uWS::SERVER>* ws = nullptr;

};
