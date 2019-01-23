/***********************************************************************
 * Program:
 *    Lab 03, Shell Lab
 *    Brother Ball, CS 345
 * Author:
 *    Ted McDaniel
 * Summary:
 *    A simple shell with signal catching and a history feature. Samples
 *    provided in the lab documents were heavily borrowed from.
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <signal.h>
#include <cstring>
#include <sstream>
#include <cstdlib>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */

// Gloabal variables to handle signal catching
#define BUFFER_SIZE 50
static char handlerMessage[BUFFER_SIZE];

// Global variables for the history functionality
#define HISTORY_SIZE 10
#define MAX_COMMANDS 1000
string commands[HISTORY_SIZE];
int numCommands = 0;

/***************************************************
 * handle_SIGQUIT
 *
 * Shows history when the SIGQUIT signal is given
 ***************************************************/
void handle_SIGQUIT(int)
{
   int lenOfMsg;
   lenOfMsg = strnlen(handlerMessage, BUFFER_SIZE);
   write(STDOUT_FILENO, handlerMessage, lenOfMsg);
   return;
}

/***************************************************
 * addToHistory()
 *
 * Adds a command to the history buffer
 ***************************************************/
void addToHistory(string input)
{
   commands[numCommands % HISTORY_SIZE] = input;
   numCommands++;

   // Overwrite old commands if we get that far
   if (numCommands >= MAX_COMMANDS)
      numCommands = HISTORY_SIZE;
}

/*******************************************************
 * findCommand()
 *
 * Finds first command that begins with character input.
 * Returns whether the command was found, or if there was
 *      an error in the search.
 *******************************************************/
bool findCommand(char a, string& stringFound)
{
   int temp = numCommands;
   stringFoudn = "";

   // If 'a' is null, return most recent command
   if (a == '\0')
   {
      if (numCommands)
      {
         stringFound = commands[(numCommands - 1) % HISTORY_SIZE ];
         return true;
      }
      else
      {
         return false;
      }
   }

   // Look through commands to find first command with that letter
   for (temp; temp >= 0 && temp != temp - HISTORY_SIZE; --temp)
   {
      if (commands[temp % HISTORY_SIZE][0] == a)
      {
         stringFound = commands[temp % HISTORY_SIZE];
         return true
      }
   }

   return false;
}

/*************************************************************************
 * displayHistory()
 *
 * Shows the histroy of commands with index of the command
 ************************************************************************/
void displayHistory()
{
   int start;

   if (numCommands > HISTORY_SIZE)
   {
      start = numCommands - HISTORY_SIZE;
   }
   else
   {
      start = 0;
   }

   for (int i = start; i < numCommands; i++)
   {
      cout << " " << setw(3) << i << "  " << commands[i % HISTORY_SIZE] << endl;
   }

   return;
}


/***************************************************************************
 * SETUP
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters.
 *
 * setup() modifies the inputBuffer creating a set of null-terminated strings
 * and places pointers into the args[] array that point to the beginning of
 * each argument.  A NULL pointer is placed in the args[] array indicating
 * the end of the argument list.  This is what is needed for using execvp().
 *
 * A ^d input at the beginning of a line, by a user, exits the shell.
 **************************************************************************/
void setup(char inputBuffer[], char * args[], int * background)
{
   int length; // # of characters in the command line 
   int i;      // loop index for accessing inputBuffer array 
   int start;  // index where beginning of next command parameter is 
   int ct;     // index of where to place the next parameter into args[] 
    
   ct = 0;
    
   /* read what the user enters on the command line */
   length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  

   start = -1;            /* Used as a flag to indicate that we are looking
                           * for the start of the command or an argument if
                           * it is a -1, or stores the starting position of
                           * the command or argument to be put in args[].
                           */
   if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
   if (length < 0)
   {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
   }

   /* examine every character in the inputBuffer */
   for (i = 0; i < length; i++)
   { 
      switch (inputBuffer[i])
      {
         case ' ':
         case '\t' :               /* argument separators */
            if(start != -1)        /* found the end of the command or arg */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */
               ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

         case '\n':                 /* should be the final char examined */
            if (start != -1)        /* if in the command or an argument */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */ 
               ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

         default :             /* some other character */
            if (start == -1 && inputBuffer[i] != '&')
               start = i;      /* starting position of the command or arg */
            if (inputBuffer[i] == '&')
            {
               *background  = 1;
               inputBuffer[i] = '\0';
            }
      }  /* end of switch */
   }  /* end of for loop looking at every character */
   args[ct] = NULL; /* just in case the input line was > 80 */
} 

int main(void)
{
   char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
   int background;             /* equals 1 if a command is followed by '&' */
   char *args[MAX_LINE/2];   /* command line (of 80) has max of 40 arguments */

   // Set up the signal handler
   struct sigaction handler;
   handler.sa_handler = handle_SIGQUIT;
   sigaction(SIGQUIT, &handler, NULL);

   // String to verify signal catch
   strncpy(handlerMessage, "Quitting because of quitter signals.\n", BUFFER_SIZE);
   
   while (1)                   /* Program terminates normally inside setup */
   {
      background = 0;
      printf(" COMMAND-> ");
      fflush(stdout);
      setup(inputBuffer, args, &background);       /* get next command */

      
      // Fork a child. See page 118 of textbook.
      // TODO: Catch unrecognized commands
      pid_t pid;
      pid = fork();

      if (pid < 0)
      {
         printf("Fork failed.\n");
         return 1;
      }
      else if (pid == 0)
      {
         execvp(args[0], args);
      }
      else
      {
         if (background != 1)
         {
            waitpid(pid, NULL, 0);
         }
      }
   }
}

