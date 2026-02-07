#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <math.h>
#define TAILLE_BUFF 256


void erreur(const char* msg)
{
    printf("Erreur: %s\n",msg);
    close(socketC);
    exit(1);
}


void traitement_nombres(char* buffer, int* nb1, int* nb2)
{
    int nombre1[10];
    int nombre2[10];
    memset(&nombre1,-1,sizeof(nombre1));
    memset(&nombre2,-1,sizeof(nombre2));

    // positionne l'indice i sur le premier nombre, dépend du message envoyé par le serveur mais inscrit en dur ici
    int i = 171;
    // récupère le premier nombre
    int j = 0;
    while(i<strlen(buffer) && j < 10 && buffer[i] != ' ')
    {
        nombre1[j] = buffer[i];
        j++;
        i++;
    }

    i += 17;
    // récupère le second nombre
    int k = 0;
    while(i<strlen(buffer) && k < 10 && buffer[i] != ' ')
    {
        nombre2[k] = buffer[i];
        k++;
        i++;
    }

    if(j <= 0 || j >= 10 || k <= 0 || k >= 10) erreur("traitement_nombres");

    // j et k indiquent les tailles des tableaux
    for(int i=0;i<j;i++)
        *nb1 += (nombre1[i]-'0')*pow(10,j-i-1);
    for(int i=0;i<k;i++)
        *nb2 += (nombre2[i]-'0')*pow(10,k-i-1);
}

int socketC;

int main()
{
    // paramètres
    int port = 52002;
    const char* hote = "challenge01.root-me.org";
    

    // création du socket
    socketC = socket(AF_INET,SOCK_STREAM,0); // IPv4, paquets ordonnés
    if(socketC == -1) erreur("socket");


    // résolution DNS
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // paquets ordonnés

    if(getaddrinfo(hote,NULL,&hints,&res) != 0) erreur("getaddrinfo");


    // adresse IPv4 du serveur
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));

    servaddr.sin_family = AF_INET; // type IPv4
    servaddr.sin_port = htons(port); // port libre choisi
    servaddr.sin_addr = ((struct sockaddr_in*) res->ai_addr)->sin_addr; // IPv4 obtenue via DNS
    freeaddrinfo(res);
    

    // ouverture de la connexion socketClient <-> socketServeur
    // struct sockaddr: type général des adresses
    if(connect(socketC,(struct sockaddr*) &servaddr,sizeof(servaddr)) != 0) erreur("connect");

    // lecture de deux nombres fournis par le serveur
    char buff[TAILLE_BUFF];
    memset(&buff,0,sizeof(buff));
    read(socketC,buff,sizeof(buff));
    /* message de la forme:
    ====================
    GO BACK TO COLLEGE
    ====================
    You should tell me the answer of this math operation in less than 2 seconds !

    Calculate the square root of 799 and multiply by 6223 =
    */
    printf("Message:\n%s\n\n",buff);
    int nb1 = 0; int nb2 = 0;
    traitement_nombres(buff,&nb1,&nb2);
    printf("Nombre 1: %i\tnombre 2: %i\n",nb1,nb2);


    // calcul du résultat à envoyer
    double resultat = sqrt(nb1)*nb2;
    resultat = round(resultat*100)/100.0;
    // récupération du résultat en string
    char buff_resultat[32];
    memset(&buff_resultat,0,sizeof(buff_resultat));
    snprintf(buff_resultat,sizeof(buff_resultat),"%.2f",resultat);
    strcat(buff_resultat,"\n");
    printf("Résultat: %s\n",buff_resultat);
    write(socketC,buff_resultat,strlen(buff_resultat));


    // lecture du code secret en réponse
    char secret[TAILLE_BUFF];
    memset(&secret,0,sizeof(secret));
    read(socketC,secret,sizeof(secret));
    printf("Code secret: %s\n",secret);

    close(socketC);
}