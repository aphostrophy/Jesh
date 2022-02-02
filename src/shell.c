/***************************************************************************//**

  @file         shell.c

  @author       Jesson Yo

  @date         Tuesday,  1 February 2022

  @brief        JESH (Jess SHell)

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

void jesh_loop(void)
{
    char *line;
    char **args;
    int status;

    do{
        printf("> ");
        line = jesh_read_line();
        // args = jesh_split_line(line);
        // status = lsh_execute(args);

        free(line);
        // free(args);
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