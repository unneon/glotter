#pragma once

#include <bits/stdc++.h>
#include <uWS/uWS.h>

class Glotter {

	class Waiter {
	public:
		void wait() {
			std::unique_lock<std::mutex> lck(mtx);
			cv.wait(lck);
		}
		void notify() {
			cv.notify_one();
		}
	private:
		std::condition_variable cv;
		std::mutex mtx;
	};

public:

	enum class LogLevel { nothing = 0, important = 1, debug = 2 };
	friend bool operator>=(LogLevel a, LogLevel b) { return static_cast<int>(a) >= static_cast<int>(b); }

    Glotter(){
		if (auto reqLL = getenv("GLOTTER_LOGLEVEL")) {
			using namespace std::string_literals;
			if (reqLL == "nothing"s) ll = LogLevel::nothing;
			else if (reqLL == "important"s) ll = LogLevel::important;
			else if (reqLL == "debug"s) ll = LogLevel::debug;
			else log(LogLevel::important, [&]{ std::clog << "Unknown log level\"" << reqLL << "\""; });
		}

        uws.onHttpRequest([&](uWS::HttpResponse* res, uWS::HttpRequest req, char* /*data*/, size_t /*len*/, size_t /*remain*/){
            log(LogLevel::debug, [&]{ std::clog << (req.getMethod() == uWS::METHOD_GET ? "GET " : "(unknown method) ") << req.getUrl().toString(); });
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
        uws.onConnection([&, this](uWS::WebSocket<uWS::SERVER>* conn, auto){
            this->log(LogLevel::important, []{ std::clog << "Connected"; });
            ws = conn;
        });
		uws.onDisconnection([&, this](uWS::WebSocket<uWS::SERVER>*, int, char*, size_t){
			this->log(LogLevel::important, []{ std::clog << "Disconnected"; });
		});
		uws.onMessage([&, this](uWS::WebSocket<uWS::SERVER>*, char* msg, size_t len, uWS::OpCode) {
			if (std::string(msg, len) == "unpause") { this->pausemech.notify(); }
			else { this->log(LogLevel::important, [&]{ std::clog << "Unrecognized message!"; }); }
		});
        uws.listen(port);
        std::thread([&]{uws.run();}).detach();
		log(LogLevel::debug, [&]{ std::clog << "Listening on port " << port; });
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
	void setLogLevel(LogLevel newll) { ll = newll; }
	template <typename F> void log(LogLevel priority, F f) { if (ll >= priority) { std::clog << "[GLOTTER] "; f(); std::clog << std::endl; } }

private:

    static std::string readFile(const std::string& path) {
        std::ifstream file;
        file.exceptions(std::ios::badbit | std::ios::failbit);
        file.open(path, std::ios::binary);
        std::stringstream buf;
        buf << file.rdbuf();
		return buf.str();
    }
    void send(const std::string& s) {
        if (!ws) {
			log(LogLevel::important, [&]{ std::clog << "Waiting for connection at port " << port; });
            while (!ws)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
		log(LogLevel::debug, [&]{ std::clog << s; });
        ws->send(s.c_str(), s.size(), uWS::OpCode::TEXT);
		ws->send("pause", 5, uWS::OpCode::TEXT);
		pausemech.wait();
    }

    uWS::Hub uws;
    uWS::WebSocket<uWS::SERVER>* ws = nullptr;
	LogLevel ll = LogLevel::important;
	unsigned short port = 57077;
	Waiter pausemech;

};
