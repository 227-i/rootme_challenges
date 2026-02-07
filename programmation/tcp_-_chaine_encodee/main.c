#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define TAILLE_BUFF 256

int socketC;

void erreur(const char* msg)
{
    printf("Erreur: %s\n",msg);
    close(socketC);
    exit(1);
}

void base64_to_ascii(char* source, char* res)
{
    base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i = 0;
    int offsets[TAILLE_BUFF];
    memset(offsets,-1,sizeof(offsets));

    // on cherche l'offset (0-63) des caractères de source par rapport à la base
    while(i < strlen(source))
    {
        int j = 0;
        while(j < strlen(cle) && cle[j] != source[i])
            j++;
        if(j < strlen(cle))
            offsets[i] = j;
        i++;

    }

    int decalage = 0;
    if(source[strlen(source)-1] == '=') decalage +=2;
    if(source[strlen(source)-2] == '=') decalage +=2;

    offsets[i] = offsets[i] >> decalage;

    // TODO: mettre les bits côte à côte pour reformer des mots de 8 bits afin de les traduire en ascii
}

int main()
{
    // paramètres
    int port = 52023;
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


    char buff[TAILLE_BUFF];
    memset(&buff,0,sizeof(buff));
    read(socketC,buff,sizeof(buff));
    printf("%s\n",buff);
    /*


    char buff_resultat[32];
    memset(&buff_resultat,0,sizeof(buff_resultat));
    snprintf(buff_resultat,sizeof(buff_resultat),"%.2f",resultat);
    strcat(buff_resultat,"\n");
    printf("Résultat: %s\n",buff_resultat);
    write(socketC,buff_resultat,strlen(buff_resultat));

    char secret[TAILLE_BUFF];
    memset(&secret,0,sizeof(secret));
    read(socketC,secret,sizeof(secret));
    printf("Code secret: %s\n",secret);*/

    close(socketC);
}