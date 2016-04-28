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
int fin_partie(char *mot, int taille_mot,int nb_erreur)
{
	if (nb_erreur ==0)
		return 1;
	int i =0;
	for (i =0;i<taille_mot;i++)
	{
		if (mot[i]=='_')
			return 0;
	}
	// si on arrive ici, la partie est finie
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
	// si on arrive ici, on n'a pas trouvé le char c dans le mot
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
void init(char *mot_trouve , int taille_mot, char value)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		mot_trouve[i]=value;
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

/* Procedure correspondant au traitemnt du serveur de votre application */

{
	int taille_tampon = 10; // taille du tampon pour communiquer
	
	int lettre[26]; // vaut 0 si la lettre n'a pas été joué , lettre[0] correspond au char 'a'...
	
	char nb_erreurs = 5; // nombre d'erreurs commises, initialiser a 5 et décrementer a chaque erreur
	
	char taille_mot = 5; // taille du mot a trouver, pour l'instant il est entree en dur
	
	char mot[taille_mot]; // le mot a trouver
	
	int tdonne[10];
	
	mot[0]='s';
	mot[1]='a';
	mot[2]='l';
	mot[3]='u';
	mot[4]='t';
	char *c;
	char mot_trouve[taille_mot]; //le mot découvert par l'utilisateur, initialiser avec des '_' au début
	char partie_finie =0; // detecte la fin de partie
	int nb_requete_max = 1; // nombre de requete max
	int socket_pendu; // socket permettant de communiquer avec notre client
	char tampon[taille_tampon]; // permet de récupérer les entrées du client
	
	struct sockaddr_in p_adr_socket; 
	struct sockaddr_in p_adr_utilisateur;
	
	init(mot_trouve,taille_mot,'_');
	
	init_l(lettre,26,0);
	
	int socket= h_socket(AF_INET,SOCK_STREAM); // mettre AF_INET apres cree la socket

	adr_socket(service,"localhost",protocole,&p_adr_socket); //affecte p_adr_socket
	
	h_bind (socket,&p_adr_socket); // bind la socket
	
	h_listen(socket,nb_requete_max); // attente d'une connection
	socket_pendu=h_accept(socket,&p_adr_utilisateur); // accepte la connection
	
	//h_reads(socket_pendu,tampon,10);
	//printf("affichage de test : %c \n",tampon[0]);
	
	//sprintf(c,"%d",taille_mot);//c[0] == taille_mot
	//h_writes(socket_pendu,c,sizeof(char)); // envoie la taille du mot au client
	while (!partie_finie)
	{
		h_reads(socket_pendu,tampon,sizeof(char));// lit le char tappé en entrée
		printf("test read %c\n",tampon[0]);
		if(tampon[0]=='\n')
			NULL;
		else
		{
			if((tampon[0] <='z' && tampon[0] >='a') || ( tampon[0] <='Z' && tampon[0] >='A'))
			{
				if(!(verif_lettre(lettre,tampon[0])))// verifie que le char en entrée n'a pas été donné
				{
					printf("verif lettre");
					maj_lettre(lettre,tampon[0]);
					if(est_lettre_mot(mot,taille_mot,tampon[0]))// verifie que le char est dans le mot
					{
						printf("verif est_lettre");
						apparaitre_lettre_mot(mot,mot_trouve,taille_mot,tampon[0]); // fait apparaitre ce char dans le mot_trouve
					}
					else
						nb_erreurs --; // la lettre n'est pas dans le mot, FROMAGE
				}
				else nb_erreurs --;// cette lettre est a déjà été donné , FROMAGE encore
			}
			else
				nb_erreurs --; // Il faut donner un char en entree ...
		}
		int i;
		printf("test mot_trouve serveur : ");
		for (i=0;i<taille_mot;i++)
			printf("%c",mot_trouve[i]);
		partie_finie = fin_partie(mot_trouve,taille_mot,nb_erreurs);
		h_writes(socket_pendu,mot_trouve,taille_mot);// envoie l'état de mot_trouve actuel
		
		//sprintf(c,"%d",nb_erreurs);// convertie nb_erreurs en char pour l'envoyé au client
		
		h_writes(socket_pendu,&nb_erreurs,sizeof(char)); // envoie le nombre d'erreurs restantes
		//sprintf(c,"%d",partie_finie); // convertie partie_finie en char pour l'envoyé au client
		h_writes(socket_pendu,&partie_finie,sizeof(char));	// envoie le bool de fin de partie
	}
		
			
	h_close(socket_pendu);
	h_close(socket);
}

/******************************************************************************/	

