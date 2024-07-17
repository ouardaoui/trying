#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

struct Request {
  std::string filename ;
  std::string name ;
  std::string data ;
  std::string method;
  std::string path ; 
  std::string version;
  std::map <std::string , std::string> headers;
};

std::string trim(const std::string& str) {
    std::string result = str;

    if (result.length() >= 2 && result.substr(0, 2) == "\r\n") {
        result = result.substr(2);
    }
    if (result.length() >= 2 && result.substr(0, 2) == "\r\n") {
        result = result.substr(2);
    }

    if (result.length() >= 2 && result.substr(result.length() - 2) == "\r\n") {
        result = result.substr(0, result.length() - 2);
    }

    return result;
}


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

Request sub_parse(std::istringstream &stream, std::string start, std::string end, Request &req)
{
  std::string line ;
  std::string tmp = "";
   while (line.find(start) == std::string::npos)
    std::getline(stream, line);
   while(std::getline(stream, line))
  {
      if (line.find("Content-Disposition: form-data") != std::string::npos) {
        int i = line.find("name=") + 6;
        req.name = "";
        while(line[i] != '"' && line[i] != '\r' && line[i] != '\r' &&  line[i] != '\0')
          req.name += line[i++];         
        i = line.find("filename=") + 10;
        req.filename = "";
        while(line[i] != '"' && line[i] != '\r' && line[i] != '\r' &&  line[i] != '\0')
          req.filename += line[i++];
      }
      else if(line.find("Content-Type:") != std::string::npos)
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
    req.data = trim(req.data);
   return req;

}
    


Request ft_parse(std::string str)
{
    std::istringstream stream(str);
    Request req ;
    stream >> req.method >> req.path >> req.version;
    req.headers =  parseHeaders(stream);
    std::string start = Start(str);
    std::string end = start + "--";
    if(str.find("multipart/form-data") != std::string::npos)
    {
      sub_parse(stream,start, end, req);
    }
    return req;
}



int main(int argc, char **argv){
    std::ifstream    multi_body;
    std::string    multi_data;

    if (argc < 2)
        return (1);

    multi_body.open(std::string(argv[1]));
    if (multi_body.is_open() == false){
        std::cerr << "cant open : "  << argv[1] << std::endl;
        return (1);
    }
    std::stringstream strStream;
    strStream << multi_body.rdbuf();
    multi_data = strStream.str();
    Request req =  ft_parse(multi_data);
    std::string path = "data/"+req.filename ;
    std::cout << req.filename ;
    std::ofstream out(path, std::ios::binary);
    out.write(req.data.data(), req.data.size());
    out.close();
    return 0;
}



