/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/	

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include<sys/signal.h>
#include<sys/wait.h>

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "udp"

void serveur_appli (char *service, char* protocole);   /* programme serveur */


/******************************************************************************/	
/*---------------- programme serveur ------------------------------*/

main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */
	char *protocole= PROTOCOLE_DEFAUT; /* protocole par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  printf("defaut protocole = %s\n", protocole);
		  break;
 	case 2:
		  service=argv[1];
		  printf("defaut protocole = %s\n", protocole);
		  break;
 	case 3 :
		  service=argv[1];
		  protocole=argv[2];
		  break;

   	default :
		  printf("Usage:serveur service (nom ou port) protocole (TCP ou UDP)\n");
		  return EXIT_SUCCESS;
 	}

	/* service est le service (ou numŽro de port) auquel sera affectŽ
	ce serveur*/
	/* protocole est le protocole qui sera utilisŽ */
	
	serveur_appli(service, protocole);
}
// renvoie 1 si la partie est fini , 0 sinon
char fin_partie(char *mot, int taille_mot,int nb_erreur)
{
	if (nb_erreur ==0)
		return 1;
	int i =0;
	for (i =0;i<taille_mot;i++)
	{
		if (mot[i]<'a' || mot[i] >'z')
			return 0;
	}
	
	return 1;
}
// verifie si le char c a déja été joué ou non
int verif_lettre(int *lettre, char c)
{
	int i;
	i = c - 'a'; // a check
	printf("i = %d",i);
	return lettre[i];
}
void maj_lettre(int *lettre, char c)
{
	int i;
	i = c - 'a'; // a check
	lettre[i]==1;
}
//verifie que la lettre est dans le mot
int est_lettre_mot (char *mot ,int taille_mot, char c)
{
	int i=0;
	for(i=0;i<taille_mot;i++)
	{
		if (mot[i]==c)
			return 1;
	}
	
	return 0;
}
void apparaitre_lettre_mot(char *mot, char *mot_trouve, int taille_mot, char c)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		if(mot[i]==c)
			mot_trouve[i]=c;
	}
}
// init le tableau en parametre avec la valeur value
void init(char *mot , int taille_mot, char value)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		mot[i]=value;
	}
}
void init_l(int *lettre , int taille_mot, int value)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		lettre[i]=value;
	}
}
/******************************************************************************/	
void serveur_appli(char *service, char *protocole)
{
	int taille_tampon = 10; // taille du tampon pour communiquer
	
	int lettre[26]; // vaut 0 si la lettre n'a pas été joué , lettre[0] correspond au char 'a'...
	
	char nb_coups = 5; 	
	char taille_mot = 5; // taille du mot a trouver, pour l'instant il est entree en dur	
	char mot[taille_mot]; // le mot a trouver
	mot[0]='s';
	mot[1]='a';
	mot[2]='l';
	mot[3]='u';
	mot[4]='t';	
	char mot_trouve[taille_mot]; //le mot connu du joueur

	char *c;
	
	char partie_finie =0; // detecte la fin de partie
	int nb_requete_max = 1; // nombre de requete max
	int socket_pendu; // socket permettant de communiquer avec notre client
	char tampon[taille_tampon]; // permet de récupérer les entrées du client
	
	struct sockaddr_in p_adr_socket; 
	struct sockaddr_in p_adr_utilisateur;
	
	init(mot_trouve,taille_mot,'.');
	
	init_l(lettre,26,0);
	
	int socket= h_socket(AF_INET,SOCK_STREAM); // mettre AF_INET apres cree la socket

	adr_socket(service,"localhost",protocole,&p_adr_socket); //affecte p_adr_socket
	
	h_bind (socket,&p_adr_socket); // bind la socket
	
	h_listen(socket,nb_requete_max); // attente d'une connection
	socket_pendu=h_accept(socket,&p_adr_utilisateur); // accepte la connection
	

	h_writes(socket_pendu,&taille_mot,sizeof(char)); // envoie la taille du mot au client

	while (!partie_finie)
	{
		h_reads(socket_pendu,tampon,sizeof(char));// lit le char tappé en entrée
		printf("test read %c\n",tampon[0]);
					
		if(!(verif_lettre(lettre,tampon[0]))) // La lettre n'a pas été donnée
		{
			maj_lettre(lettre,tampon[0]);
			if(est_lettre_mot(mot,taille_mot,tampon[0]))// La lettre est dans le mot
				apparaitre_lettre_mot(mot,mot_trouve,taille_mot,tampon[0]); // MAJ de la lettre dans le mot_trouve
			
			else
				nb_coups --; 
		}
		else 
			nb_coups --;
		
		
		partie_finie = fin_partie(mot_trouve,taille_mot,nb_coups);

		h_writes(socket_pendu,mot_trouve,taille_mot);// envoie l'état de mot_trouve actuel
					
		h_writes(socket_pendu,&nb_coups,sizeof(char)); // envoie le nombre d'erreurs restantes
		
		h_writes(socket_pendu,&partie_finie,sizeof(char));	// notifie l'état de la partie
	}
		
			
	h_close(socket_pendu);
	h_close(socket);
}

/******************************************************************************/	

