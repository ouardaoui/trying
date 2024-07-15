#include <iostream>

int main()
{
  std::string str = "index?num=1&num=2";

  std::string query = str.substr(str.find("?") + 1);
  std::string path = str.substr(0,str.find("?"));

  std::cout << query << std::endl;
  std::cout << path << std::endl;
}
