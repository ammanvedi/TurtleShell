#include "main.h"

char *current_directory;
int ** environment_variables;

int main(void)
{
    readProfile();
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
        printf("%s", buf);
        //determine if reading the path variable line
        if(strstr(buf, "PATH") != NULL)
        {
            //if we are reading the path line, then count the number of
            //directories, so we can create an appropriatley sized array
            printf("found the path line\n");
            
            char *pathpointslash = 1;
            char *pathpointcolon = buf;
            
            while((pathpointslash != NULL) && (pathpointcolon != NULL) )
            {
                pathpointslash = strchr(pathpointcolon+1, '/');
                pathpointcolon = strchr(pathpointslash+1, ':');
                
                printf("found slash at %s\n", pathpointslash);
                printf("found colon at %s\n", pathpointcolon);
            }
        }else
        {
            if(strstr(buf, "HOME") != NULL)
            {
                printf("found the home line\n");
            }
        }
    }
    
    return 0;
}

int init(char* startpathname)
{
    return 0;
}


int gotodir(char* togo)
{
    return 0;
}