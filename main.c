#include "main.h"

char *current_directory;
char * env_path;
char * env_home;
char **pathfolder;

int main(void)
{
    
    init("/Users/ammanvedi");
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
            int pathcount = 0;
            
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
                    
                    char *pathvar = malloc(sizeof(char)*(strlen(pathpointslash)-strlen(pathpointcolon)));
                    
                    strncpy(pathvar, pathpointslash, strlen(pathpointslash)-strlen(pathpointcolon));
                    
                    pathfolder[foldercount] = pathvar;
                    
                    printf(" FOLDERED PATHVAR ::  %s\n", pathfolder[foldercount]);
                    
                    foldercount+=1;
                    
                    
                }
            }
            
            env_path = *pathfolder;
            
            
        }else
        {
            if(strstr(buf, "HOME") != NULL)
            {
                env_home = strchr(buf, '/');
                
                
                printf(" FOLDERED HOME :: %s\n", env_home);
            }
        }
    }
    
    return 0;
}

int init(char* startpathname)
{
    readProfile();
    
    int x = 0;
    
    //for(x = 0; x < 
    
    
    for(x = 0; x < 15; x++)
    {
        printf("the first path variable is %s\n", pathfolder[x]); 
    }
    
    return 0;
}


int gotodir(char* togo)
{
    return 0;
}