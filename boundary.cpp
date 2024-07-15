#include <iostream>
#include <sstream>
#include <map>

std::string Start(const std::string & request) {
  std::string boundary = "";
  int i = request.find("boundary=") + 9;
  while (request[i] != '\r' && request[i] != '\n' && request[i] != '\0') {
    boundary += request[i];
    i++;
  }
  return boundary;
}
std::map<std::string, std::string> parseHeaders(std::istringstream& stream) {
    std::map<std::string, std::string> headers;
    std::string line;
    
    getline(stream,line) ;
    if(line != "\r")
    {
      size_t pos = line.find(": ");
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos + 2); 
      headers[key] = value;
    }
    while (std::getline(stream, line) && line != "\r") {
        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2); 
            headers[key] = value;
        }
    }

    return headers;
}

struct Request {
  std::string filename = "";
  std::string name = "";
  std::string data = "";
};


Request parseshiiit(std::istringstream &stream, std::string start, std::string end)
{
  Request req ;
  std::string line ;
  std::string tmp = "";
   while (line.find(start) == std::string::npos)
    std::getline(stream, line);
   while(std::getline(stream, line))
  {
      if(line.find("filename") != std::string::npos)
      {
        int i = line.find("filename=") + 10;
        req.filename = "";
        while(line[i] != '"' && line[i] != '\r' && line[i] != '\r' &&  line[i] != '\0')
          req.filename += line[i++];
      }
      else if (line.find("Content-Disposition: form-data") != std::string::npos) {
        int i = line.find("name=") + 6;
        req.name = "";
        while(line[i] != '"' && line[i] != '\r' && line[i] != '\r' &&  line[i] != '\0')
          req.name += line[i];         
      }
      else if(line.find("Content-Type:"))
        continue;
      else if (line.find(end) != std::string::npos) {
        req.data += tmp;
        break;
      }
      else if (line.find("--" + start) != std::string::npos)
        continue;
      else 
        req.data += line + "\n";
  }
   std::cout << line << "\n";
   return req;

}
    

int main() {
    std::string boundary = "boundary_string_here";
    std::string filename = "example.txt";
    std::string fileContent = "Hello, world!\n"; // Example content of the file

    std::ostringstream request;
    request << "POST /upload HTTP/1.1\r\n";
    request << "Host: example.com\r\n";
    request << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n";
    
    size_t contentLength = 0;
    contentLength += 2 + boundary.size() + 2; // "--boundary_string_here\r\n"
    contentLength += 14; // "Content-Type: text/plain\r\n"
    contentLength += 2 + fileContent.size() + 2; // "\r\n... contents of example.txt file ...\r\n"
    contentLength += 2 + boundary.size() + 4; // "--boundary_string_here--\r\n"

    request << "Content-Length: " << contentLength << "\r\n";
    request << "\r\n";

    request << "--" << boundary << "\r\n";
    request << "Content-Disposition: form-data; name=\"file\"; filename=\"" << filename << "\"\r\n";
    request << "Content-Type: text/plain\r\n";
    request << "\r\n";
    request << fileContent << "\r\n";
    request << "--" << boundary << "--\r\n";

    std::string req = request.str();
    std::string method;
    std::string path;
    std::string version;
    std::istringstream stream(req);
    stream >> method >> path >> version;
    std::map<std::string, std::string> headers = parseHeaders(stream);
    std::string start = Start(req);
    std::string end = start + "--";
    std::cout << req ;
    if(req.find("multipart/form-data") != std::string::npos)
    {
      Request HttpReq = parseshiiit(stream,start, end);
    }

    return 0;
}

