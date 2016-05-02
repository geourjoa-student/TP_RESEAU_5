
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include<sys/signal.h>
#include<sys/wait.h>

#include "fon.h"     

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"

#define TAILLE_TAMPON 10
#define NB_REQUETE_MAX 1

void serveur_appli (char *service, char* protocole);   

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
		  printf("Usage:\n> serveur service protocole\n");
		  return EXIT_SUCCESS;
 	}

	
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
// verifie si le char c a d�ja �t� jou� ou non
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
	int lettres[26]; // lettres[i] vaut 0 si la ieme lettre n'a pas �t� jou�e
	
	char nb_coups = 5, taille_mot=7 , mot[]="reseaux", mot_trouve[taille_mot], partie_finie = 0,tampon[TAILLE_TAMPON];
	
	int socket_pendu; 
		
	struct sockaddr_in p_adr_socket; 
	struct sockaddr_in p_adr_utilisateur;
	
	init(mot_trouve,taille_mot,'.');
	
	init_l(lettres,26,0);
	
	// Cr�ation socket
	int socket= h_socket(AF_INET,SOCK_STREAM); 

	adr_socket(service,"localhost",protocole,&p_adr_socket); 
	
	// Association de la socket
	h_bind (socket,&p_adr_socket); 
	
	//Mise en �coute de la socket
	h_listen(socket,NB_REQUETE_MAX); // 
	
	//Acceptation de connexion
	socket_pendu=h_accept(socket,&p_adr_utilisateur); 
	

	h_writes(socket_pendu,&taille_mot,sizeof(char)); // envoie la taille du mot au client

	while (!partie_finie)
	{
		h_reads(socket_pendu,tampon,sizeof(char));// lit le char tapp� en entr�e
						
		if(!(verif_lettre(lettres,tampon[0]))) // La lettre n'a pas �t� donn�e
		{
			maj_lettre(lettres,tampon[0]);
			if(est_lettre_mot(mot,taille_mot,tampon[0]))// La lettre est dans le mot
				apparaitre_lettre_mot(mot,mot_trouve,taille_mot,tampon[0]); // MAJ de la lettre dans le mot_trouve
			
			else
				nb_coups --; 
		}
		else 
			nb_coups --;
		
		
		partie_finie = fin_partie(mot_trouve,taille_mot,nb_coups);

		h_writes(socket_pendu,mot_trouve,taille_mot);// envoie l'�tat de mot_trouve actuel
					
		h_writes(socket_pendu,&nb_coups,sizeof(char)); // envoie le nombre d'erreurs restantes
		
		h_writes(socket_pendu,&partie_finie,sizeof(char));	// notifie l'�tat de la partie
	}
		
			
	h_close(socket_pendu);
	h_close(socket);
}

/******************************************************************************/	

