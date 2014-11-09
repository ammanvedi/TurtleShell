/********************************************************

                        MAIN.C

  Amman Vedi - UCL Computer Science - Operating Systems
                
            C O U R S E W O R K    O N E  
          
                   NOVEMBER 2014
                   
            More Information in README.MD

*********************************************************/


#include "main.h"

char * current_directory;
char * env_path;
char * env_home;
char **pathfolder;
//keep a count of haw many string paths are
//being stored in the pathfolder pointer array
int pathcount = 0;

int main(void)
{
    
    init();
    return 0;
}   

/*

Take the input string of space delimited arguments
and generate an array of char * to each argument

Args: buf - Pointer to the  buffer containing the path string 
Returns char * Pointer to the char * array

*/

char* parsePathString(char * buf)
{
                
            //if we are reading the path line, then count the number of
            //directories, so we can create an appropriatley sized array
           
            
            char *pathpointslash = (char *)'0';
            char *pathpointcolon = buf;
            pathcount = 0;
            
            while((pathpointcolon != NULL) )
            {
                pathpointslash = strchr(pathpointcolon+1, '/');
                pathpointcolon = strchr(pathpointslash+1, ':');

                if((pathpointslash != NULL))
                {
                    
                    pathcount+=1;
                    //printf("COUNTED PATHVAR ::  %d\n", pathcount);
                }else
                {
                    //printf("no next slash\n");
                }
            }
            
            pathfolder = (char **) malloc(sizeof(char *) * pathcount);
            
            int foldercount = 0;
            pathpointslash = (char*)'0';
            pathpointcolon = buf;
    
            //iterate and fill the array with the path variables
            //paths are between / and : characters
            
            while((pathpointcolon != NULL) )
            {
                pathpointslash = strchr(pathpointcolon+1, '/');
                pathpointcolon = strchr(pathpointslash+1, ':');
                
                if((pathpointslash != NULL))
                {
                    
                    
                    if(pathpointcolon == NULL)
                    {
                    //the last path item, dont reference pathpointcolon
                    //reference point for string changes to total string length
                    char *pathvarnull = malloc(sizeof(char)*((strlen(buf)-strlen(pathpointslash))+1));
                    strncpy(pathvarnull, pathpointslash, strlen(buf)-strlen(pathpointslash));
                    //null terminate string, strcpy doesnt always do this
                    pathvarnull[strlen(pathvarnull)] = '\0';
                    pathfolder[foldercount] = pathvarnull;

                    }else
                    {
                    //printf("madeit\n");
                    char *pathvar = malloc(sizeof(char)*((strlen(pathpointslash)-strlen(pathpointcolon))+1));
                    strncpy(pathvar, pathpointslash, strlen(pathpointslash)-strlen(pathpointcolon));
                    pathvar[strlen(pathvar)] = '\0';
                    pathfolder[foldercount] = pathvar;
                    }
                    foldercount+=1;
                    
                    
                }
            }
            return *pathfolder;
}


/*

Open the local .profile file, read the Home and Path variables
and save these locally

Returns 0 if successfull, 1 if an error was thrown

*/

int readProfile()
{
    FILE *profilepointer;
    char buf[1000];
    char *temphome;

    
    profilepointer = fopen(".profile", "rt");
    
    if(profilepointer == NULL)
    {
        printf("\x1b[31mERROR :: \x1b[0m Profile not found \n");
        return 1;
    }
    
    while(fgets(buf,1000, profilepointer) != NULL)
    {
        //determine if reading the path variable line
        if(strstr(buf, "PATH") != NULL)
        {
            //parse the path string
            env_path = parsePathString(buf);
      
        }else
        {
            if(strstr(buf, "HOME") != NULL)
            {
                temphome = strchr(buf, '/');
                env_home = malloc(sizeof(char)*strlen(temphome));
                strcpy(env_home, temphome);
            }
        }
    }
    
    //verify both path and home have been assigned
    //path is assigned if pathcount > 0
    //home is assigned if it has been assigned
    
    if( (pathcount < 1)  )
    {
        printf("\x1b[31mERROR :: \x1b[0m Could not read PATH \n");
        return 1;
    }
    
    if( strlen(env_home) < 1)
    {
        printf("\x1b[31mERROR :: \x1b[0m Could not read HOME \n");
        return 1;
    }
    
    
    return 0;
}

/*

System Iniitialisation,

Read profile from local directory and switch to the home directory.
Begin reading commands

*/

int init()
{
    if(readProfile() == 1)
    {
        return 0;
    }
    changeDirectory(env_home);
    while(1)
    {
        readCommand();
    }
    
    //printf("changing directory to home ststus %d\n",);
    return 0;
}

/*

Print the current working directory and then wait for input
from the user. parse the input arguments, and pass the array to 
runprogram

Args: Arguments[] - pointer to arguments array;
                        
                    {Progname , arg0, arg1 , arg2...}
                    
                    initial item must be executable 
                    name of program
    
*/

int readCommand()
{
    
    char *command = malloc(sizeof(char)*30);
    char *arguments = malloc(sizeof(char)*100);
    char *arg_token;
    const char argsplitter = ' '; 
    char *svp;
    char **arg_list = malloc(sizeof(char*)*2);
    char *tempstring;
    int countargs = 0;
    
  
    
    printf("%s >> ", current_directory);
    //read any character except a newline, consume any character straight after 
    //this will allow the reading of a null terminated string without the 
    //newline character (consumed regardless by %*c
    scanf("%[^\n]%*c",arguments);

    
    if(strlen(arguments) < 2)
    {

        return 1;
    }
    
    //split arguments

    arg_token = strtok_r(arguments, &argsplitter, &svp);
    
    while(arg_token != NULL)
    {
        //reallocate the argument list
        arg_list = realloc(arg_list, sizeof(char *)*(++countargs));
        arg_list[countargs-1] = (char *)malloc(strlen(arg_token)+1);
        strcpy(arg_list[countargs-1], arg_token);
        arg_token = strtok_r(NULL, &argsplitter, &svp);        
    }
    
    arg_list = realloc(arg_list, sizeof(char *)*(countargs+1));
    arg_list[countargs] = (char*)0;
    
    runProgram(arg_list);    
    free(command);
    free(arguments);
    
    return 0;
    
}

/*

Take a set of arguments and a program name, locate the program
in the system, fork a new process and run the requestd program

The C library cd command uses chdir also, so this is intercepted 
and chdir is called locally

assignments to $PATH and $HOME in the local environment are 
also checked for here.

Args: Arguments[] - pointer to arguments array;
                        
                    {Progname , arg0, arg1 , arg2...}
                    
                    initial item must be executable 
                    name of program
    
*/

int runProgram(char *arguments[])
{

    char *foundprogpath = findProgram(arguments[0]);
    int c_stat;
    
    //if the user is attempting to change directory 
    //change directory in local environment
    if(strcmp("cd", arguments[0]) == 0)
    {
        changeDirectory(arguments[1]);
        return 0;
    }
    
    char *hometest;
    char *pathtest;
    
    if( (hometest = strnstr(arguments[0], "$HOME=", 6)) != NULL)
    {
        //user wishes to assign a new home
        free(env_home);
        char *newhome = malloc(sizeof(char)*strlen(hometest));
        strcpy(newhome, hometest+6);
        env_home = newhome;
        return 0;
        
    }
    
    if( ( pathtest = strnstr(arguments[0], "$PATH=", 6) ) != NULL)
    {
        //user wishes to assign a new path
        //printf("want to edit path \n");
        free(env_path);
        env_path = parsePathString(pathtest+6);
        
        return 0;
        
    }

    if(foundprogpath != NULL)
    {
        //found a program in one of the
        //path variable paths
        //printf("FOUND REQUESTED EXE ::%s %d\n", foundprogpath, strlen(foundprogpath));
        //fork the current process
        int newproc = fork();

        if(newproc == 0)
        {
            int execstat = execv(foundprogpath, arguments);
            printf("calling EXECV failed :: %d\n", execstat);
            
        }else
        {

            pid_t tpid;

            do 
            {
                tpid = wait(&c_stat);
            }while(tpid != newproc);
        }
    }
    

    free(arguments);
    free(foundprogpath);
    return 0;
}

/*

Search all paths within PATH variable, check for executable file
with name matching programname

Args: programname - pointer to string containing name of program to be run
Returns: Null if no executable found, Pointer to the path of the matched 
         executable

*/


char* findProgram(char* programname)
{
    
    struct dirent *directoryinfo;
    struct stat storestat;
    DIR *direc;
    char fullname[100];
    
    int y = 0;
    
    for(y = 0; y < (pathcount - 1); y++)
    {
        
        if((direc = opendir(pathfolder[y])) == NULL)
        {
            //couldnt open directory
            return NULL;
        }        
        //find all programs in the directory
        while((directoryinfo = readdir(direc)) != NULL)
        {
            sprintf( fullname , "%s/%s",pathfolder[y],directoryinfo->d_name);
            if(stat(fullname, &storestat) == 0)
            {
                //check if file executable
                if(access(fullname,X_OK) == 0)
                {   
                    //check its name comparing to the desired program
                    if(strcmp(directoryinfo->d_name,programname) == 0 )
                    {

                            char *allocatedfullname = malloc(sizeof(char)*(strlen(fullname)+1));
                            strcpy(allocatedfullname, fullname);
                            allocatedfullname[strlen(fullname)] = '\0';
                            return allocatedfullname;
                            
                    }
                }
            }
        }    
    }
    
    return NULL;
}

/*

Change the working directory, and save a reference to it in 
variable current_directory

Args: togo - Pointer to the path string of directory to go to 
Returns binary result of chdir

*/

int changeDirectory(char *togo)
{
    
    char *formatteddir;
    formatteddir = malloc(sizeof(char)*(strlen(togo)));
    strcpy(formatteddir, togo);
    formatteddir[strlen(togo)-1] = '\0';
    int x = chdir(formatteddir);
    
    if(x == 0)
    {
        //chdir success
        char cwd[512];
        //refresh local copy of current directory with a 
        //call to getcwd
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            char *fullpath = malloc(sizeof(char)*(strlen(cwd)));
            strcpy(fullpath, cwd);
            free(current_directory);
            current_directory = fullpath;

        }
    }
    return x;
}