/***************************************************************************//**

  @file         shell.c

  @author       Jesson Yo

  @date         Tuesday,  1 February 2022

  @brief        JESH (Jess SHell)

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>


void jesh_loop(void)
{
    printf("> ");
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