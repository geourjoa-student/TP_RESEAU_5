/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/	


#include <stdio.h>
#include <stdlib.h>
#include <curses.h> 		/* Primitives de gestion d'Žcran */
#include <sys/signal.h>
#include <sys/wait.h>
#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "udp"
#define SERVEUR_DEFAUT "localhost"

void client_appli (char *serveur, char *service, char *protocole);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
	char *protocole= PROTOCOLE_DEFAUT; /* protocole par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
		case 1 :		/* arguments par defaut */
			  printf("serveur par defaut: %s\n",serveur);
			  printf("service par defaut: %s\n",service);
			  printf("protocole par defaut: %s\n",protocole);
			  break;
		case 2 :		/* serveur renseigne  */
			  serveur=argv[1];
			  printf("service par defaut: %s\n",service);
			  printf("protocole par defaut: %s\n",protocole);
			  break;
		case 3 :		/* serveur, service renseignes */
			  serveur=argv[1];
			  service=argv[2];
			  printf("protocole par defaut: %s\n",protocole);
			  break;
		case 4:			/* serveur, service, protocole renseignes */
			  serveur=argv[1];
			  service=argv[2];
			  protocole=argv[3];
			  break;
		default:
			  printf("Usage: \n> client ip  port  protocole \n");
			  return EXIT_SUCCESS;
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	/* protocole le protocole qui sera utilise pour la communication */
	
	client_appli(serveur,service,protocole);
}
void viderbuffer()
{
	int c;
	while((c=getchar()) != EOF && c!='\n');
}
/*****************************************************************************/
void client_appli (char *serveur,char *service,char *protocole)
{
	int i;
	
	char nb_erreurs;
	
  	char c[10]; // test de chiasse
  	
	int taille_mot; // taille du mot a trouvé
	
	int jeu_fini=0; // indicateur de jeu fini
	
	char Tampon[10]; 
	
	struct sockaddr_in p_adr_distant;
	
	//Création socket
	int socket_local = h_socket(AF_INET,SOCK_STREAM); // mettre AF_INET apres
	
	adr_socket(service,serveur,protocole,&p_adr_distant);// initialise p_adr_distant
	
	//Connexion au serveur
	h_connect(socket_local,&p_adr_distant);
	
	//h_reads(socket_local,c,sizeof(char)); // recupere la taille du mot
	//printf("c[0] = %c ",c[0]);
	//taille_mot= c[0] -'0';
	taille_mot=5;
	printf("taille mot : %d \n",taille_mot);
	char mot_trouve[taille_mot]; // declaration du tableau du mot_courant
	printf("\nBienvenue dans le jeu du pendu !\n");
	while(!jeu_fini)
	{
		printf("Donnez une lettre \n");
		scanf("%c",&c[0]);// lit le char
		viderbuffer();
		printf("c[0] : %c",c[0]);
		
		h_writes(socket_local,&c[0],sizeof(char)); // envoie le char au serveur
		h_reads(socket_local,mot_trouve,taille_mot); // lit l'état du mot
		h_reads(socket_local,Tampon,sizeof(char)); // recupere le nombre d'erreur
		
		
		//nb_erreurs= Tampon[0] -'0';
		nb_erreurs= Tampon[0];
		
		printf("Voici le mot actuel : \n");
		for(i=0;i<taille_mot;i++)
		printf("%c",mot_trouve[i]);
		printf("\n Vous avez encore droit a %d erreurs\n",nb_erreurs);
		h_reads(socket_local,Tampon,sizeof(char)); // recupere le boolean de fin de partie
		jeu_fini=Tampon[0];
	}
	


	h_writes(socket_local,Tampon,10);
	h_close(socket_local);
	

 }

/*****************************************************************************/

