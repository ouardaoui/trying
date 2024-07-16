#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>


// h = 1 if post 
// h = 0 if get


std::string cgi_handler(std::string name,std::string cgi_path , int h, std::string body, char **env)
{
    std::string response;
    int status;
    int fdin[2];
    pid_t pid;
    int start = time(0);
    
    if (pipe(fdin) == -1) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0)
    {
        if (h == 1) //  we have post  
        {
            if (dup2(fdin[0], 0) == -1)
            {
                perror("dup2");
                exit(1);
            }
        }
        if (dup2(fdin[1], 1) == -1)
        {
            perror("dup2");
            exit(1);
        }

        close(fdin[0]);
        close(fdin[1]);

        char **av = new char *[3];
        av[0] = new char[cgi_path.length() + 1];
        av[1] = new char[std::strlen(name.c_str()) + 1];
        av[2] = NULL;
        std::strcpy(av[0], cgi_path.c_str());
        std::strcpy(av[1], name.c_str());
        if (execve(cgi_path.c_str(), av, env) == -1) // set env instance of NULL
        {
            perror("execve");
            exit(127);
        }
        exit(0);
    }
    else
    {
        if (h == 1)
        {
            if(write(fdin[1], body.c_str(), body.length()) == -1);
                std::perror("Error writing to file descriptor");
        }
        int result;
        while ((result = waitpid(pid, &status, WNOHANG)) == 0)
        {
            if (result == 0)
            {
                if ((time(0) - start) > 4)
                {
                    response = "HTTP/1.1 500 Internal Server Error";
                    kill(pid, SIGKILL);
                    close(fdin[1]);
                    return response;
                }
                sleep(1);
            }
        }
        if (WEXITSTATUS(status) != 0)
        {
            return "";
        }
        close(fdin[1]);
        char buffer[1024];
       
        while (true)
        {
            int len = read(fdin[0], buffer, 1023);
            if (len <= 0)
                break;
            if (len == 1023)
                buffer[len] = '\0';
            else
                buffer[len + 1] = '\0';
            if (len >= 0)
            {
                buffer[len] = '\0';
                response += buffer;
            }
        }
        return response;
    }
    return "";
}
/*
        env[0] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
		env[1] = ft_strdup("REQUEST_METHOD=GET");
		query = "QUERY_STRING=" + query;
		env[2] = (char*)query.c_str();
		std::string scriptFileName = "SCRIPT_FILENAME=" + scriptPath;
		env[3] = (char *)scriptFileName.c_str();
		env[4] = NULL;
*/


int main()
{
     std::string body = "username=abass&password=23";
    std::string query = body;
    std::string cgi_path = "/usr/bin/python3";
    std::string name = "./cgi/login.py";
    char *env[5];
    env[0] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	env[1] = strdup("REQUEST_METHOD=POST");
	//query = "QUERY_STRING=" + query; //fro get
	//env[2] = (char*)query.c_str(); 
    std::string bodyLen = "CONTENT_LENGTH=26";
    env[2] = (char *)bodyLen.c_str();
	std::string scriptFileName = "SCRIPT_FILENAME=" + name;
	env[3] = (char *)scriptFileName.c_str();
	env[4] = NULL;
    //std::string cgi_path = "/usr/bin/python3";
    //std::string name = "./cgi/script.py";
    std::string out = cgi_handler(name , cgi_path,1, body, env);
    std::cout << out ;
}