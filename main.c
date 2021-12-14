#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int test;

char *getField( int fn, char line[], char field[] ){
    int i;
    char *p, aux[256];
    strcpy(aux, line );
    p = strtok( aux, "," );
    for ( i = 0; i < fn; i++ ) p = strtok( NULL, "," );
    return strcpy(field, p);
}
void main(){
    char buf[300];
    char linia[300];
    int a = 0, i = 0, j = 0, b = 0, comes = 0; 
    int fd = open("CAVIDEOSCURT.csv", O_RDONLY);
    ssize_t prova = read(fd, buf, 299);
    buf[300] = '\0';
    while(a==0)
    {
        if (buf[i] == ',')
        {
            comes += 1;
        }
        if (comes == 15){
            break;
        }
        if (comes == 11){
            b = 0;
            while (b=0){
                i+=1;
                if (buf[i] == ','){
                    a = 1;
                } 
            }
        }
        else{
            linia[j] = buf[i];
            j += 1;
        }
        i += 1;
    }
    linia[300] = '\0';
    printf("%s \n", linia);
}






