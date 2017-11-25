#include <mutex>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <sys/stat.h>
#include "../include/glotter.hpp"

void Glotter::Waiter::wait() {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
}
void Glotter::Waiter::notify() {
    cv.notify_one();
}

std::string Glotter::readFile(const std::string& path) {
    std::ifstream file;
    //file.exceptions(std::ios::failbit);
    file.open(path, std::ios::binary);
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

bool operator>=(Glotter::LogLevel a, Glotter::LogLevel b) {
    return static_cast<int>(a) >= static_cast<int>(b);
}

Glotter::Glotter(){
    if (auto reqLL = getenv("GLOTTER_LOGLEVEL")) {
        if (reqLL == std::string("nothing")) ll = LogLevel::nothing;
        else if (reqLL == std::string("important")) ll = LogLevel::important;
        else if (reqLL == std::string("debug")) ll = LogLevel::debug;
        else log(LogLevel::important, [&]{ std::clog << "Unknown log level\"" << reqLL << "\""; });
    }
    auto webResourcePrefix = std::string(".");
    if (std::getenv("GLOTTER_WEBRESPREFIX") != nullptr)
        webResourcePrefix = getenv("GLOTTER_WEBRESPREFIX");
    else if (pathExists("/usr/share/glotter/web"))
        webResourcePrefix = "/usr/share/glotter/web";
    else if (pathExists("/usr/local/share/glotter/web"))
        webResourcePrefix = "/usr/local/share/glotter/web";
    log(LogLevel::important, [&]{ std::clog << "Using " << webResourcePrefix << " as web resource directory"; });

    uws.onHttpRequest([webResourcePrefix, this](uWS::HttpResponse* res, uWS::HttpRequest req, char* /*data*/, size_t /*len*/, size_t /*remain*/){
        log(LogLevel::debug, [&]{ std::clog << (req.getMethod() == uWS::METHOD_GET ? "GET " : "(unknown method) ") << req.getUrl().toString(); });
        if (req.getMethod() == uWS::METHOD_GET) {
            auto sharefile = [&](const char* alias, const std::string& name){
                if (req.getUrl().toString() == alias) {
                    auto r = readFile(name);
                    res->end(r.c_str(), r.size());
                }
            };
            sharefile("/", webResourcePrefix + "/index.html");
            sharefile("/style.css", webResourcePrefix + "/style.css");
            sharefile("/glotter.js", webResourcePrefix + "/glotter.js");
            sharefile("/bebas.ttf", webResourcePrefix + "/BEBAS___.ttf");
            sharefile("/jquery.js", webResourcePrefix + "/jquery.min.js");
            sharefile("/vis.css", webResourcePrefix + "/vis.min.css");
            sharefile("/vis.js", webResourcePrefix + "/vis.min.js");
        }
    });
    uws.onConnection([&, this](uWS::WebSocket<uWS::SERVER>* conn, const uWS::HttpRequest&){
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

void Glotter::addEdge1(int a, int b){
    send("addEdge1 " + std::to_string(a) + " " + std::to_string(b));
}
void Glotter::addEdge2(int a, int b){
    send("addEdge2 " + std::to_string(a) + " " + std::to_string(b));
}
void Glotter::setEdge1Color(int a, int b, const std::string& color) {
    send("setEdge1Color " + std::to_string(a) + " " + std::to_string(b) + " " + color);
}
void Glotter::setEdge2Color(int a, int b, const std::string& color) {
    send("setEdge2Color " + std::to_string(a) + " " + std::to_string(b) + " " + color);
}
void Glotter::setVertexColor(int v, const std::string& color) {
    send("setVertexColor " + std::to_string(v) + " " + color);
}
void Glotter::resize(int n){
    send("resize " + std::to_string(n));
}
void Glotter::setLogLevel(LogLevel newll) {
    ll = newll;
}
template <typename F> void Glotter::log(LogLevel priority, F f) {
    if (ll >= priority) {
        std::clog << "[GLOTTER] ";
        f();
        std::clog << std::endl;
    }
}

void Glotter::send(const std::string& s) {
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

bool Glotter::pathExists(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

template <typename T> T coalesce(const T& a, const T& b) { return a ? a : b; }