#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <MLV/MLV_all.h>
#include "structures.h"
#include "fonctions.c"
#include "graphismes.c"

int jouer(Liste lst, int temps_ecoule, int score, int couleur, int forme) {
	MLV_Image* background = affiche_background();

	int clic_x, clic_y;
	int i;
	int a_insere = 0, a_decale = 0;
	int temps_depart = MLV_get_time() - temps_ecoule * 1000;
	int temps_restant;
	int type;
	int reponse;
	int combo;
	Token *token1, *token2, *token3, *token4, *token5;
	Zone *clic_tokens = NULL;
	Zone decalage_couleur, decalage_forme;

	// Dans le cas où on a chargé une partie
	if (lst != NULL)
		dessine_liste(&clic_tokens, background, lst);

	// On affiche les boutons 'INSERER'
	Zone inserer_gauche, inserer_droite;
	cree_boutons_insertion(&inserer_gauche, &inserer_droite);

	// Affiche le bouton Pause et récupère sa zone de clic
	Zone pause;
	affiche_bouton_pause(&pause);

	if ( couleur == -1 && forme == -1 ) {
		couleur = MLV_get_random_integer(0, 4);
		forme = MLV_get_random_integer(0, 4);
	}

	// Affichage du token suivant en haut, avec une couleur et une forme aléatoire
	dessine_suivant(background, couleur, forme);

	// Boucle principale
	while ( 1 ) {

		// On attend que l'utilisateur clique quelque part, ou que le temps soit écoulé
		while ( 1 ) {

			// On affiche le score
			affiche_score(score, background);

			// On affiche et on récupère le temps restant
			temps_restant = affiche_temps_restant(temps_depart, background);

			a_decale = 0;

			if (a_insere) {
				disparait_suivant(background, couleur, forme);
				// Si l'utilisateur a inséré un token, on affiche le suivant
				couleur = MLV_get_random_integer(0, 4);
				forme = MLV_get_random_integer(0, 4);

				dessine_suivant(background, couleur, forme);
				a_insere = 0;
			}

			MLV_wait_mouse_or_seconds(&clic_x, &clic_y, temps_restant);

			// S'il clique sur un bouton 'INSERER'
			if ( len(lst) < 11 ) {
				if ( inserer_gauche.x1 < clic_x && clic_x < inserer_gauche.x2 && inserer_gauche.y1 < clic_y && clic_y < inserer_gauche.y2 ) {
					ajouter_a_gauche(&lst, couleur, forme, 1);
					a_insere = 1;
					// On réinitialise les coordoonées
					clic_x = clic_y = -1;
					break;
				} else if ( inserer_droite.x1 < clic_x && clic_x < inserer_droite.x2 && inserer_droite.y1 < clic_y && clic_y < inserer_droite.y2 ) {
					ajouter_a_droite(&lst, couleur, forme, 1);
					a_insere = 1;
					// On réinitialise les coordoonées
					clic_x = clic_y = -1;
					break;
				}
			}

			// S'il clique sur Pause
			if ( pause.x1 < clic_x && clic_x < pause.x2 && pause.y1 < clic_y && clic_y < pause.y2 ) {
				temps_restant = affiche_temps_restant(temps_depart, background);
				reponse = affiche_pause(&temps_depart);
				if (reponse == 1) {
					sauvegarder(lst, 120 - temps_restant, score, couleur, forme);
				} else if (reponse == 2) {
					libere_liste(&lst);
					MLV_free_image(background);
					return affiche_fin(score);
				}
			}

			temps_restant = affiche_temps_restant(temps_depart, background);

			// S'il clique sur un des tokens existants, i étant le numéro du token en partant de 0 et de la gauche
			for (i=0; i < len(lst); i++) {
				if (clic_tokens[i].x1 < clic_x && clic_x < clic_tokens[i].x2 && clic_tokens[i].y1 < clic_y && clic_y < clic_tokens[i].y2) {
					// On affiche les boutons de décalage et on récupère leur zone pour déterminer la zone de clique
					decalage_couleur = dessine_decalage_couleur(clic_tokens[i], (*token_indice(lst, i)).couleur);
					decalage_forme = dessine_decalage_forme(clic_tokens[i], (*token_indice(lst, i)).forme);
					// On attend que l'utilisateur clique soit sur le bouton de decalage des couleurs, soit sur celui des formes, soit ailleurs
					MLV_wait_mouse_or_seconds(&clic_x, &clic_y, temps_restant);
					if ( decalage_couleur.x1 < clic_x && clic_x < decalage_couleur.x2 && decalage_couleur.y1 < clic_y && clic_y < decalage_couleur.y2 ) {
						circu_token(background,lst,clic_tokens,0,(*token_indice(lst, i)).couleur);
						decalage_circulaire_couleur(lst, (*token_indice(lst, i)).couleur);
						a_decale = 1;
					} else if ( decalage_forme.x1 < clic_x && clic_x < decalage_forme.x2 && decalage_forme.y1 < clic_y && clic_y < decalage_forme.y2 ) {
						circu_token(background,lst,clic_tokens,1,(*token_indice(lst, i)).forme);
						decalage_circulaire_forme(lst, (*token_indice(lst, i)).forme);
						a_decale = 1;
					}
					// Dans tous les cas, après qu'il ait cliqué, on efface les boutons de décalage
					efface_decalage_couleur(background);
					efface_decalage_forme(background);
					// On réinitialise la boucle pour que l'utilisateur puisse cliquer sur un autre token directement
					i = -1;
					// On réinitialise les coordoonées
					clic_x = clic_y = -1;
				}
			}

			if (a_decale)
				break;

			// Fin de la partie
			if (temps_restant <= 0) {
				libere_liste(&lst);
				MLV_free_image(background);
				return affiche_fin(score);
			}
		}

		// Fin de la partie
		if (temps_restant <= 0) {
			libere_liste(&lst);
			MLV_free_image(background);
			return affiche_fin(score);
		}

		if (a_insere || a_decale) {
			// On affiche la liste
			dessine_liste(&clic_tokens, background, lst);

			// On regarde s'il y a 3 tokens consécutifs ayant la même couleur ou la même forme
			combo = 1;
			for (i=0; i < 4; i++) {
				// On met un while et pas un if car il peut y avoir plusieurs suppressions successives
				type=max(detecte_couleurs_consecutives(lst, i, &token1, &token2, &token3),detecte_formes_consecutives(lst, i, &token1, &token2, &token3));
				while (type) {
					if (verif_max_consecutives(lst,type,&token1,&token3,&token4,&token5)==5){
						disparait_token(background, clic_tokens, lst, 5, token1, token2, token3, token4, token5);
						free( extrait_token(&lst, token1) );
						free( extrait_token(&lst, token2) );
						free( extrait_token(&lst, token3) );
						free( extrait_token(&lst, token4) );
						free( extrait_token(&lst, token5) );
						score += 70 * combo;
					}else if (verif_max_consecutives(lst,type,&token1,&token3,&token4,&token5)==4){
						disparait_token(background, clic_tokens, lst, 4, token1, token2, token3, token4);
						free( extrait_token(&lst, token1) );
						free( extrait_token(&lst, token2) );
						free( extrait_token(&lst, token3) );
						free( extrait_token(&lst, token4) );
						score += 30 * combo;
					}else{
						disparait_token(background, clic_tokens, lst, 3, token1, token2, token3);
						free( extrait_token(&lst, token1) );
						free( extrait_token(&lst, token2) );
						free( extrait_token(&lst, token3) );
						score += 10 * combo;
					}
					// On réaffiche la liste à chaque suppression
					dessine_liste(&clic_tokens, background, lst);
					combo++;
					i = -1;
					type=max(detecte_couleurs_consecutives(lst, i, &token1, &token2, &token3),detecte_formes_consecutives(lst, i, &token1, &token2, &token3));
				}
			}

			// On affiche la liste
			dessine_liste(&clic_tokens, background, lst);
		}
	}
}

int menu() {
	int clic_x, clic_y;
	Zone jouer, charger, options, quitter;

	affiche_menu(&jouer, &charger, &options, &quitter);

	while ( 1 ) {
		MLV_wait_mouse(&clic_x, &clic_y);

		if (jouer.x1 < clic_x && clic_x < jouer.x2 && jouer.y1 < clic_y && clic_y < jouer.y2)
			return 1;
		else if (charger.x1 < clic_x && clic_x < charger.x2 && charger.y1 < clic_y && clic_y < charger.y2)
			return 2;
		else if (options.x1 < clic_x && clic_x < options.x2 && options.y1 < clic_y && clic_y < options.y2)
			return 3;
		else if (quitter.x1 < clic_x && clic_x < quitter.x2 && quitter.y1 < clic_y && clic_y < quitter.y2)
			return 4;
	}
}

int main(int argc, char const *argv[]) {

	int largeur_fenetre = 1280, hauteur_fenetre = 720;
	int menu_principal = 0;

	int temps_ecoule, score, couleur, forme, rejouer;
	Liste lst;

	MLV_create_window("Three to go", "Three to go", largeur_fenetre, hauteur_fenetre);

	do {
		temps_ecoule = 0;
		score = 0;
		couleur = -1;
		forme = -1;
		lst = NULL;
		switch(menu()) {
			case 1: // Jouer
					while(jouer(lst, temps_ecoule, score, couleur, forme));
					menu_principal = 1;
					break;
			case 2: // Charger
					if ( !charger(&lst, &temps_ecoule, &score, &couleur, &forme) ) {
						affiche_erreur_charger();
						menu_principal = 1;
						break;
					}
					do {
						rejouer = jouer(lst, temps_ecoule, score, couleur, forme);
						temps_ecoule = 0;
						score = 0;
						couleur = -1;
						forme = -1;
						lst = NULL;
					} while( rejouer );
					menu_principal = 1;
					break;
			case 3: // Options
					affiche_options();
					menu_principal = 1;
					break;
			case 4: // Quitter
					MLV_free_window();
					return 0;
		}
	} while ( menu_principal );

	MLV_free_window();

	return 0;
}
