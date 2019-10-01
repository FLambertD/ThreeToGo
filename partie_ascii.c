#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <MLV/MLV_all.h>
#include "structures.h"
#include "fonctions.c"
#include "graphismes.c"

void affiche_liste_ascii(Liste lst, char couleurs[], char formes[]) {

	printf("[  ");

	if (lst != NULL) {
		Liste debut = lst;

		do {
			printf("--> ");
			printf("%c", formes[(*lst).forme]);
			printf("%c ", couleurs[(*lst).couleur]);
			lst = (*lst).suivant;
		} while ( lst != debut );

		printf("-->>");
	}

	printf("  ]\n");
}

int string_length(char string[]) {
	int compteur = 0;

	while ( string[compteur] != '\0' )
		compteur++;

	return compteur;
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));
	
	printf("----- Bienvenue sur Three to go ! -----\n");

	char couleurs[] = {'r', 'v', 'b', 'j'};
	char formes[] = {'O', 'C', 'T', 'D'};

	char reponse;
	int token_forme, token_couleur;
	int i;
	int rejouer;
	Token *token1;
	Token *token2;
	Token *token3;
	Liste lst;

	do {
		printf("Alignez 3 tokens ayant un caractère en commun pour cumuler des points\n");
		printf("---------------------------------------------------------------------\n\n");
		lst = NULL;
		do {
			token_forme = rand() % string_length(formes);
			token_couleur = rand() % string_length(couleurs);
			printf("Le prochain token est (%c%c)\n", formes[ token_forme ], couleurs[ token_couleur ]);
			affiche_liste_ascii(lst, couleurs, formes);
			printf("'A' pour placer le token à gauche || 'Z' pour placer le token à droite\n");
			printf("'Q' pour quitter\n");
			do {
				if ( !scanf("%c", &reponse) )
					printf("Erreur, veuillez réessayer\n");
			} while ( reponse != 'a' && reponse != 'A' && reponse != 'z' && reponse != 'Z' && reponse != 'q' && reponse != 'Q');
			if ( reponse == 'a' || reponse == 'A' )
				ajouter_a_gauche(&lst, token_couleur, token_forme, 0);
			else if ( reponse == 'z' || reponse == 'Z' )
				ajouter_a_droite(&lst, token_couleur, token_forme, 0);
			else
				break;
			for (i=0; i < string_length(couleurs); i++ ) {
				while ( detecte_couleurs_consecutives(lst, i, &token1, &token2, &token3) || detecte_formes_consecutives(lst, i, &token1, &token2, &token3) ) {
					free( extrait_token(&lst, token1) );
					free( extrait_token(&lst, token2) );
					free( extrait_token(&lst, token3) );
					i = 0;
				}
		}
		} while ( 1 );

		do {
			printf("\n");
			printf("Voulez-vous rejouer ?  1 - OUI  ||  0 - NON\n");
			if ( !scanf("%d", &rejouer) )
				printf("Erreur, veuillez réessayer\n");
		} while ( rejouer != 1 && rejouer != 0 );
		printf("\n");
	} while ( rejouer );

	libere_liste(&lst);

	return 0;
}