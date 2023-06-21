#define CROW_MAIN
#include "crow.h"
#include <vector>
#include <map>

std::vector<std::string> notes{"yo bro"};

// Function to parse URL-encoded strings
std::map<std::string, std::string> parse_urlencoded(std::string str) {
    std::map<std::string, std::string> result;
    std::string key, value;
    bool isKey = true;

    for (char c : str) {
        if (c == '=') {
            isKey = false;
        } else if (c == '&') {
            result[key] = value;
            key.clear();
            value.clear();
            isKey = true;
        } else {
            if (isKey) {
                key += c;
            } else {
                value += c;
            }
        }
    }
    result[key] = value;

    return result;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/notes")
        .methods("GET"_method, "POST"_method, "OPTIONS"_method)
        ([&](const crow::request& req) {
            if (req.method == "POST"_method) {
                auto data = parse_urlencoded(req.body);
                auto it = data.find("note");
                if (it != data.end()) {
                    notes.push_back(it->second);
                }
                auto res = crow::response(200);
                res.set_header("Access-Control-Allow-Origin", "*");
                return res;
            } else if (req.method == "GET"_method) {
                crow::json::wvalue x;
                for (size_t i = 0; i < notes.size(); i++) {
                    x[i] = notes[i];
                }
                auto res = crow::response(x.dump());
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            } else if (req.method == "OPTIONS"_method) {
                auto res = crow::response(200);
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
                res.set_header("Access-Control-Allow-Headers", "Content-Type");
                return res;
            }
            return crow::response(404);
        });

    app.port(8080).multithreaded().run();
}
