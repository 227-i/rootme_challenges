#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define TAILLE_BUFF 80


void erreur(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    // paramètres
    int port = 52002;
    const char* hote = "challenge01.root-me.org";
    

    // création du socket
    int socketC;
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

    // TODO: traitement
    printf("Done\n");

    close(socketC);
}