#include "main.h"

char *current_directory;
char * env_path;
char * env_home;
char **pathfolder;
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
    
    
    profilepointer = fopen(".profile", "rt");
    //file open now
    
    while(fgets(buf,1000, profilepointer) != NULL)
    {
        //printf("%s", buf);
        //determine if reading the path variable line
        if(strstr(buf, "PATH") != NULL)
        {
            //if we are reading the path line, then count the number of
            //directories, so we can create an appropriatley sized array
            printf("found the path line\n");
            
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
                    //printf("found slash at %s\n", pathpointslash);
                    
                    char *pathvar = malloc(sizeof(char)*((strlen(pathpointslash)-strlen(pathpointcolon))+1));
                    
                    strncpy(pathvar, pathpointslash, strlen(pathpointslash)-strlen(pathpointcolon));
                    
                    //printf("%d bytes\n", strlen(pathvar));
                    
                    //null terminate string, strcpy doesnt always do this
                    pathvar[strlen(pathvar)] = '\0';
     
                    pathfolder[foldercount] = pathvar;
                    
                    printf(" FOLDERED PATHVAR ::  %s\n", pathvar);
                    
                    foldercount+=1;
                    
                    
                }
            }
            
            env_path = *pathfolder;
            
            
        }else
        {
            if(strstr(buf, "HOME") != NULL)
            {
                env_home = strchr(buf, '/');
                
                
                //printf(" FOLDERED HOME :: %s\n", env_home);
            }
        }
    }
    
    return 0;
}

int init()
{
    readProfile();
    
    int x = 0;    
    readCommand();
    
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
    
    scanf("%s %[^\t\n]", command, arguments);
    printf("%s %s\n", command, arguments);
    
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
    
    
    
    //check arg length
    
    
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
    
    //printf("%s\n", pathfolder[1]);
    
    int y = 0;
    
    for(y = 0; y < (pathcount - 1); y++)
    {
        
        if((direc = opendir(pathfolder[y])) == NULL)
        {
            printf("didnt open directory\n");
            return 1;
        }

        printf("opened directory %s\n", pathfolder[y]);
        
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

int changeDirectory(char* togo)
{
    return 0;
}