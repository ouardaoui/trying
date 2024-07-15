#include <iostream>
#include <sstream>
#include <string>
#include <map>

std::map<std::string, std::string> parseHeaders(std::istringstream& stream) {
    std::map<std::string, std::string> headers;
    std::string line;

    while (std::getline(stream, line) && line != "\r") {
        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2); // Skip ": "
            headers[key] = value;
        }
    }

    return headers;
}

int main() {
    // Example HTTP request headers
    std::istringstream stream("Host: example.com\r\n"
                              "Content-Type: text/plain\r\n"
                              "Content-Length: 123\r\n"
                              "\r\n");

    // Parse headers into a map
    std::map<std::string, std::string> headers = parseHeaders(stream);

    // Print the parsed headers
    for (const auto& pair : headers) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}

