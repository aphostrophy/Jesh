/***************************************************************************//**

  @file         shell.c

  @author       Jesson Yo

  @date         Tuesday,  1 February 2022

  @brief        JESH (Jess SHell)

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
    Function Declarations for builtin shell commands:
*/
int jesh_cd(char **args);
int jesh_help(char **args);
int jesh_exit(char **args);

/*
    List of builtin commands, followed by their corresponding functions.
*/
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &jesh_cd,
    &jesh_help,
    &jesh_exit
};

int jesh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
    Builtin function implementations
*/
int jesh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else{
        if(chdir(args[1]) != 0){
            perror("jesh");
        }
    }
    return 1;
}

int jesh_help(char **args)
{
    int i;
    printf("Jesson Yo's JESH\n");
    printf("Jess Bash, version 0.0.1");
    printf("These shell commands are defined internally. Type 'help' to see this list");

    for (i=0; i < jesh_num_builtins(); i++){
        printf("  %s\n", builtin_str[i]);
    }
}

int jesh_exit(char **args)
{
    return 0;
}

#define JESH_RL_BUFSIZE 1024
/**
 * @brief read_line, doesn't work with newline as it instantly terminates
 * @return array of characters
*/
char* jesh_read_line(void)
{
    int bufsize = JESH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if(buffer == NULL){
        fprintf(stderr, "jesh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(true){
        c = getchar();

        if (c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else{
            buffer[position] = c;
        }
        position++;

        if(position >= bufsize) {
            bufsize += JESH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if(buffer == NULL){
                fprintf(stderr, "jesh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define JESH_TOK_BUFSIZE 64
#define JESH_TOK_DELIM " \t\r\n\a"
char **jesh_split_line(char *line)
{
    int bufsize = JESH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if(tokens == NULL){
        fprintf(stderr, "jesh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, JESH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += JESH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(tokens == NULL){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, JESH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int jesh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid < 0){
        perror("jesh");
    } else if (pid == 0){
        // execvp internally calls exit if it doesn't return a value
        if (execvp(args[0], args) == -1){
            perror("jesh");
            exit(EXIT_FAILURE);
        }
    } else{
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int jesh_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < jesh_num_builtins(); i++){
        if (strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }

    return jesh_launch(args);
}

void jesh_prompt(void)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s> ", cwd); 
    } else{
        perror("lsh");
    }
}

void jesh_loop(void)
{
    char *line;
    char **args;
    int status;

    do{
        jesh_prompt();
        line = jesh_read_line();
        args = jesh_split_line(line);
        status = jesh_execute(args);

        free(line);
        free(args);
    } while(status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  jesh_loop();

  return EXIT_SUCCESS;
}