/********************************************************

                        MAIN.C

  Amman Vedi - UCL Computer Science - Operating Systems
                
            C O U R S E W O R K    O N E  
          
                  NOVEMBER 2014

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

int readProfile()
{
    FILE *profilepointer;
    char buf[1000];
    char * temphome;

    
    profilepointer = fopen(".profile", "rt");
    
    while(fgets(buf,1000, profilepointer) != NULL)
    {
        //determine if reading the path variable line
        if(strstr(buf, "PATH") != NULL)
        {
            //if we are reading the path line, then count the number of
            //directories, so we can create an appropriatley sized array
            //printf("found the path line\n");
            
            char *pathpointslash = '0';
            char *pathpointcolon = buf;
            pathcount = 0;
            
            while((pathpointslash != NULL) && (pathpointcolon != NULL) )
            {
                pathpointslash = strchr(pathpointcolon+1, '/');
                pathpointcolon = strchr(pathpointslash+1, ':');
                
                if((pathpointslash == NULL) || (pathpointcolon == NULL))
                {
                    
                }else
                {
                    pathcount+=1;
                    //printf("COUNTED PATHVAR ::  %d\n", pathcount);
                }
            }
            
            pathfolder = (char **) malloc(sizeof(char *) * pathcount);
            
            int foldercount = 0;
            pathpointslash = '0';
            pathpointcolon = buf;
            
            while((pathpointslash != NULL) && (pathpointcolon != NULL) )
            {
                pathpointslash = strchr(pathpointcolon+1, '/');
                pathpointcolon = strchr(pathpointslash+1, ':');
                
                if((pathpointslash == NULL) || (pathpointcolon == NULL))
                {
                    
                }else
                {
                    char *pathvar = malloc(sizeof(char)*((strlen(pathpointslash)-strlen(pathpointcolon))+1));
                    strncpy(pathvar, pathpointslash, strlen(pathpointslash)-strlen(pathpointcolon));
                    //null terminate string, strcpy doesnt always do this
                    pathvar[strlen(pathvar)] = '\0';
                    pathfolder[foldercount] = pathvar;
                    //printf(" FOLDERED PATHVAR ::  %s\n", pathvar);
                    
                    foldercount+=1;
                    
                }
            }
            
            env_path = *pathfolder;
            
        }else
        {
            if(strstr(buf, "HOME") != NULL)
            {
      
                temphome = strchr(buf, '/');
                env_home = malloc(sizeof(char)*strlen(temphome));
                strcpy(env_home, temphome);
                //printf(" FOLDERED HOME :: %s\n", env_home);
            }
        }
    }
    
    return 0;
}

int init()
{
    readProfile();
    changeDirectory(env_home);
    readCommand();
    //printf("changing directory to home ststus %d\n",);
    return 0;
}

int assignHome(char* home)
{
    return 0;
}

int assignPath(char* path)
{
    return 0;
}

int readCommand()
{
    
    char *command = malloc(sizeof(char)*30);
    char *arguments = malloc(sizeof(char)*100);
    char *arg_token;
    char *argsplitter = " \n"; 
    char *svp;
    char **arg_list = malloc(sizeof(char));
    char *tempstring;
    int countargs = 0;
    
   
    
    printf("%s >> ", current_directory);
    scanf("%s %[^\t\n]", command, arguments);
    //printf("%s %s\n", command, arguments);
    
    arg_token = strtok_r(arguments, argsplitter, &svp);
    
    while(arg_token != NULL)
    {
        //allocate a string pointer
        //tempstring = malloc(sizeof(char)*(strlen(arg_token)));
        //arg_token 
        
        arg_list[countargs] = arg_token;
        countargs+=1;
        //realloc
        realloc(arg_list, sizeof(char)*countargs);
        //printf("%p\n", arg_token);
        arg_token = strtok_r(NULL, argsplitter, &svp);
    }
    
    int x = 0;
    
    while(arg_list[x] != NULL)
    {
        //printf("iterating and found %s %d\n", arg_list[x], x);
        x++;
    }
    
    runProgram(command, &arg_list);
    
    return 0;
    
}

int runProgram(char* name, char **arguments[])
{
    char *foundprogpath = findProgram(name);
    
    if(foundprogpath != NULL)
    {
        //found a program in one of the
        //path variable paths
        printf("FOUND REQUESTED EXE :: %s\n", foundprogpath);
    }
              
    return 0;
}

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
                //return 1;
                if(access(fullname,X_OK) == 0)
                {   
                    //printf("Found executable %s\n", fullname);
                    if(strcmp(directoryinfo->d_name,programname) == 0 )
                    {
                           //printf("found matching exe\n");
                            return fullname;
                    }
                }
            }
        }    
    }
    
    return NULL;
}

int changeDirectory(char *togo)
{
    //free the old memory
    free(current_directory);
    //allocate a new block of the appropriate size
    //according to strlen of new string
    current_directory = malloc(sizeof(char)*(strlen(togo)));
    //copy new path into the gloabal path variable
    strcpy(current_directory, togo);

    //strings assigned here may have anewline character at the end
    //this will cause problems for chdir, so we replace the '\0'
    //to replace the newline
    
    current_directory[strlen(current_directory)-1] = '\0';
    int x = chdir(current_directory);
    return x;
}