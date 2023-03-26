#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // system calls?
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    char* pipe_input_text = "hello world";
    int pipedes[2]; // array to store fds for pipe input and output
    if (pipe(pipedes) < 0)  // populate pipedes - create the pipe
        exit(1);

    int pid1 = fork();
    if (pid1 < 0)
        exit(1);
    else if (pid1 == 0)
        write(pipedes[1], pipe_input_text, strlen(pipe_input_text));
    else
    {
        printf("parent after first fork\n");
        int pid2 = fork();
        if (pid2 < 0)
            exit(1);
        else if (pid2 == 0)
        {
            waitpid(pid1, NULL, 0);
            char pipe_output_text[11];
            read(pipedes[0], pipe_output_text, strlen(pipe_input_text));
            printf("msg piped from child 1, printing from child 2 = %s\n",
                    pipe_output_text);
        }
        else
        {
            printf("parent after second fork\n");
        }
    }
    return 0;
}
