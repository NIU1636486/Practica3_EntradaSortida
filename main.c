#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int dintreCometes = 0;
char hola[3000];
char fieldd[100];
int test;
char buf[1];


/*
int imprimirLinia(int numLinia, char linia[3000])
{
    int fd = open("CAvideos.csv", O_RDONLY);
    dintreCometes = 0;
    for (int j = 0; j < numLinia; j++)
    {
        memset(linia, 0, 3000);
        int i = 0, comesCount = 0;
        while (1)
        {
            ssize_t prova = read(fd, buf, 1);
            if (comesCount != 11 && comesCount != 15)
            {
                linia[i] = *buf;
                i += 1;
            }

            if (*buf == '"')
                dintreCometes = !dintreCometes;
            if (*buf == ',' && !dintreCometes)
                comesCount++;
            if (comesCount == 15)
                linia[i - 1] = '\0';
            if (*buf == '\n')
            {
                break;
            }
        }
    }
    return 1;
}
*/
char *getField(int fn, char line[], char field[])
{
    int i;
    char *p, aux[4000];
    strcpy(aux, line);
    p = strtok(aux, ",");

    for (i = 0; i < fn; i++)
        p = strtok(NULL, ",");
    return strcpy(field, p);
}

char *unaLinia(char unaLinia[3000], int file)
{
    int error = 0;
    memset(unaLinia, 0, 3000);
    int i = 0, comesCount = 0;
    while (1)
    {
        ssize_t prova = read(file, buf, 1);
        if (comesCount != 11 && comesCount != 15)
        {
            unaLinia[i] = *buf;
            i += 1;
        }

        if (*buf == '"')
            dintreCometes = !dintreCometes;
        if (*buf == ',' && dintreCometes)
            error = 1;

        if (*buf == ',' && !dintreCometes)
        {
            comesCount += 1;
        }

        if (comesCount == 15)
        {
            unaLinia[i - 1] = '\0';
        }
        if (*buf == '\n')
        {
            if (error == 1)
            {
                memset(unaLinia, 0, 3000);
                unaLinia[0] = '\0';
            }
            return unaLinia;
        }
    }
}

void main()
{
    int fd = open("CAVIDEOSCURT.csv", O_RDONLY);
    char line[3000] = {0}, visualitzacions[100] = {0};
    int sumatoriVisualitzacions = 0, count = 0, mitjanaVisualitzacions;
    for (int f = 0; f < 13; f++)
    {
        memset(visualitzacions, 0, 100);
        unaLinia(line, fd);
        if (line[0] != '\0')
        {
            getField(8, line, visualitzacions);
            sumatoriVisualitzacions += atoi(visualitzacions);
            count++;
        }
    }

    mitjanaVisualitzacions = sumatoriVisualitzacions / count;
}
