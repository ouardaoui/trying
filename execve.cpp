#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pid = fork();


    const char* argv[] = { "ls", "-l", nullptr }; // Argument list
    const char* envp[] = { nullptr }; // Environment variables

    // Replace the current process with the "ls" program
    if(pid == 0)
    {
      int file = open("ping.txt", O_WRONLY | O_CREAT, 0777);
      if(file == -1)
      {
        std::cout << "cant create file \n";
        return 2;
      }
      dup2(file, 1);
      close(file);
      execve("/bin/ls", const_cast<char* const*>(argv), const_cast<char* const*>(envp));
      perror("execve");
    }
    else 
    {
      int wstatus;
      wait(&wstatus);
      if(WIFEXITED(wstatus))
      {
        int statuscode = WIFEXITED(wstatus);
        std::cout << statuscode <<"\n";
      }
      std::cout << "secces \n";
    }
    // If execve returns, it indicates an error
    return 1; // Return error code
}

