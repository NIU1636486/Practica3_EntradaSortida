#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/*DECLARACIÓ DE VARIABLES*/
int dintreCometes = 0;
char buf[1];
char liniaMaxLikes[3000], liniaMaxDislikes[3000]; /*Línies amb més likes i més dislikes*/
char maxLikesTitle[100], maxLikesTime[100], maxLikesViews[100], maxLikesLikes[100], maxLikesDislikes[100];/*Caselles desitjades del video amb més likes*/
char maxDislikesTitle[100], maxDislikesTime[100], maxDislikesViews[100], maxDislikesLikes[100], maxDislikesDislikes[100];/*Caselles desitjades del video amb més dislikes*/
int sumatoriVisualitzacions = 0, sumatoriLikes = 0, sumatoriDislikes = 0, count = 0;
int mitjanaVisualitzacions, mitjanaLikes, mitjanaDislikes, anteriorLikes = 0, anteriorDislikes = 0;
char missatgeArxiu[3000];

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
        if (comesCount != 11 && comesCount != 15)  /*Amb aquest condicional ens assegurem que les dos columnes que no ens interessen (thumnail i descripció) no les afejim a la string de la línia.*/
        {
            unaLinia[i] = *buf;
            i += 1;
        }

        if (*buf == '"')
        {
            dintreCometes = !dintreCometes;
        }

        if (*buf == ',' && dintreCometes) /*Si hi ha una coma dintre d'una casella marquem que aquesta fila no ens interessa*/
            error = 1;
        
        if (*buf == ',' && !dintreCometes)/*Si trobem una coma que no està entre cometes vol dir que és un separador així que la contem per saber cuantes columnes portem*/
        {
            comesCount += 1;
        }
        
        if (comesCount == 15)/*Si arribem a 15 comes marquem que fins aqui arriba la string de la linia ja que la columna 16 no ens interessa.*/
        {
            unaLinia[i - 1] = '\0';
        }
        
        if (*buf == '\n' && !dintreCometes)
        {
            /*La linia sacaba quan arribem a un salt de linia que 
            no està entre cometes (a algunes descripcions hi ha salts 
            de linia i donava error aixi que si no estar entre cometes 
            ens assegurem que és realment el final de la linia)*/
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
    int fd = open("CAvideos.csv", O_RDONLY);
    char line[3000] = {0}, visualitzacions[100] = {0}, likes[100] = {0}, dislikes[100] = {0}, videoError[100] = {0};
    for (int f = 0; f < 40882; f++)
    {
        memset(visualitzacions, 0, 100);
        unaLinia(line, fd);
        if (line[0] != '\0' && f != 0) /*Si la linia no te cap error*/
        {
            printf("Analitzant linia %d\n", f + 1);
            /*Recollir likes, dislikes i visualitacions*/
            getField(7, line, visualitzacions);
            getField(8, line, likes);
            getField(9, line, dislikes);
            if (atoi(likes) > anteriorLikes)
            {
                anteriorLikes = atoi(likes);
                strcpy(liniaMaxLikes, line);
            }
            
            if (atoi(dislikes) > anteriorDislikes)
            {
                anteriorDislikes = atoi(dislikes);
                strcpy(liniaMaxDislikes, line);
            }

            sumatoriVisualitzacions += atoi(visualitzacions);
            sumatoriLikes += atoi(likes);
            sumatoriDislikes += atoi(dislikes);
            count++;
        }
    }
    close(fd);
    /*Càlculs de mitjanes*/
    mitjanaVisualitzacions = sumatoriVisualitzacions / count;
    mitjanaLikes = sumatoriLikes / count;
    mitjanaDislikes = sumatoriDislikes / count;
    /*Assignar a les variables maxLikes___ i maxDislikes___ les caselles desitjades per l'exercici 2.3*/
    getField(2, liniaMaxLikes, maxLikesTitle);
    getField(5, liniaMaxLikes, maxLikesTime);
    getField(7, liniaMaxLikes, maxLikesViews);
    getField(8, liniaMaxLikes, maxLikesLikes);
    getField(9, liniaMaxLikes, maxLikesDislikes);
    getField(2, liniaMaxDislikes, maxDislikesTitle);
    getField(5, liniaMaxDislikes, maxDislikesTime);
    getField(7, liniaMaxDislikes, maxDislikesViews);
    getField(8, liniaMaxDislikes, maxDislikesLikes);
    getField(9, liniaMaxDislikes, maxDislikesDislikes);
    /*Creem la variable missatgeArxiu amb tot el que volem escriure a l'arxiu*/
    snprintf(missatgeArxiu, sizeof(missatgeArxiu),
    "Mitjana visualitzacions: %d\nMitjana likes: %d\nMitjana dislikes: %d\nVIDEO AMB MES LIKES: \n     title: %s\n     publish_time: %s\n     views: %s\n     likes: %s\n     dislikes: %s\n ------------------------------\nVIDEO AMB MES DISLIKES: \n     title: %s\n     publish_time: %s\n     views: %s\n     likes: %s\n     dislikes: %s",
    mitjanaVisualitzacions, mitjanaLikes, mitjanaDislikes, maxLikesTitle, maxLikesTime, maxLikesViews, maxLikesLikes, maxLikesDislikes, maxDislikesTitle, maxDislikesTime, maxDislikesViews, maxDislikesLikes, maxDislikesDislikes);
    /*Obrir i escriure a l'arxiu GlobalOutput.txt*/
    int arxiuFinal = open("GlobalOutput.txt", O_WRONLY);
    write(arxiuFinal, missatgeArxiu, strlen(missatgeArxiu));
    close(arxiuFinal);
}

