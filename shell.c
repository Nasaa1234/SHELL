#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 100

// Function to handle the 'cd' command
void handle_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("shell");
        }
    }
}

// Function to execute commands
void execute_command(char **args)
{
    if (strcmp(args[0], "cd") == 0)
    {
        handle_cd(args);
    }
    else
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("shell");
        }
        else if (pid == 0)
        {
            // Child process
            if (execvp(args[0], args) == -1)
            {
                perror("shell");
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            wait(NULL);
        }
    }
}

// Function to split input into arguments
char **split_input(char *input)
{
    char **args = malloc(MAX_ARGS * sizeof(char *));
    char *token;
    int index = 0;

    if (!args)
    {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, " \t\n");
    while (token != NULL)
    {
        args[index++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[index] = NULL;

    return args;
}

// Main loop of the shell
void shell_loop()
{
    char *input;
    char **args;

    while (1)
    {
        input = readline("$ ");
        if (!input)
        {
            clearerr(stdin);
            continue;
        }

        if (*input)
        {
            add_history(input);
        }

        args = split_input(input);
        if (args[0] != NULL)
        {
            execute_command(args);
        }

        free(args);
        free(input);
    }
}

int main()
{
    // Start the shell loop
    shell_loop();
    return 0;
}
