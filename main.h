/********************************************************

                        MAIN.H

  Amman Vedi - UCL Computer Science - Operating Systems
                
            C O U R S E W O R K    O N E  
          
                  NOVEMBER 2014

*********************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


int init();
int changeDirectory(char *togo);
int readProfile();
int assignHome(char* home);
int assignPath(char* path);
int readCommand();
int runProgram(char* name, char **arguments[]);
char* findProgram(char* programname);
#endif
