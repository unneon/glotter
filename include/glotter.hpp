#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <uWS/uWS.h>

class Glotter {

    class Waiter {
    public:
        void wait();
        void notify();
    private:
        std::condition_variable cv;
        std::mutex mtx;
    };

public:

	enum class LogLevel : int {
        nothing = 0,
        important = 1,
        debug = 2
    };
	friend bool operator>=(LogLevel a, LogLevel b);

    Glotter();
    void addEdge1(int a, int b);
    void addEdge2(int a, int b);
    void setEdge1Color(int a, int b, const std::string& color);
    void setEdge2Color(int a, int b, const std::string& color);
    void setVertexColor(int v, const std::string& color);
    void resize(int n);
    void setLogLevel(LogLevel ll);
    void send(const std::string& msg);


private:

    static std::string readFile(const std::string& path);
    template <typename F> void log(LogLevel priority, F f);
    static bool pathExists(const std::string& path);

	uWS::Hub uws;
	uWS::WebSocket<uWS::SERVER>* ws = nullptr;
	LogLevel ll = LogLevel::important;
	unsigned short port = 57077;
	Waiter pausemech;

};
