/*
#include <iostream>
#include <string>


int main() {
    std::string str = "hello there\n";
    if (str.find("kk") == BIG)
        std::cout << str.find("kk") ;
    return 0;
}
*/ 

#include <iostream>
#include <sstream>
#include <string>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;
};

int main() {
    std::string request = "GET /index.html HTTP/1.1";

    std::istringstream stream(request);

    HttpRequest httpRequest;

    stream >> httpRequest.method >> httpRequest.path >> httpRequest.version;

    std::cout << "HTTP Method: " << httpRequest.method << std::endl;
    std::cout << "Path: " << httpRequest.path << std::endl;
    std::cout << "HTTP Version: " << httpRequest.version << std::endl;

    return 0;
}

/*
#include <iostream>
#include <sstream>
#include <string>

int main() {
    
    std::string request = "123 456.78 Hello";
    std::istringstream stream(request);

    int Int;
    double Double;
    std::string stringValue;

    stream >> Int;
    std::cout << Int << std::endl;

    stream >> Double;
    std::cout << Double << std::endl;

    stream >> stringValue;
    std::cout  << stringValue << std::endl;

    return 0;
}
*/

