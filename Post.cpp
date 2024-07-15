#include <iostream>
#include <fstream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>


//xxd -b txt.csv > txt.bin
/*upload_path = Sites-available/Uploads;

    Specifies the directory where uploaded files should be stored for requests to this location (/).*/

void Post(std::string content_type, std::string root_path , std::string upload_path, std::string file_name, std::string data, std::string request_path)
{
  std::string upload_dir;
  if(content_type.find("multipart/form-data") != std::string::npos)
  {
    upload_dir += root_path;
    if(root_path.back() != '/')
      upload_dir += '/';
    upload_dir += upload_path;
    if(upload_path.back() != '/')
      upload_dir += '/';
    std::ofstream out(upload_dir + file_name, std::ios::binary);
    out.write(data.data(), data.size());
    out.close();
    //send(201),
;   std::cout << "yes\n";
  }
  else
  {
    std::string full_path = root_path + request_path;
    struct stat Stat;
    if (stat(full_path.c_str(), &Stat) != 0)
    {
      std::cout << "404 Not Found\n"; // send response
      return ;
    }
    else if(S_ISDIR(Stat.st_mode))// check if the file exists
    {
      std::cout << "sure i will call get \n";
      //just call get in this situation ;
    }
    else if (request.path.find(".py") != std::string::npos)
    {
      std::string res = "runnnn cgiiii";
      std::cout << res << std::endl;
      if(res == "")
        std::cout << "500 internal servder error\n";
      else 
        // use send 
        
        // use send 

    }
    else if(request_path.find(".py") == std::string::npos)
      //use get;
  }
  //std::cout << upload_dir << std::endl;
};

int main()
{
  std::string content_type = "hello";
  //std::string content_type = "multipart/form-data; boundary=---------------------------7d62f37a2058";
  std::string data = "hello there\n";
  std::string root_path = "./";
  std::string upload_dir = "data";
  std::string file_name  = "output.csv";
  std::string request_path = "dat/";
  Post(content_type, root_path, upload_dir, file_name,data, request_path);
};
