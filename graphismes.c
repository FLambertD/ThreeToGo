#include <math.h>

MLV_Image* affiche_background() {
	// Affiche l'image 'background.jpg' qui couvre toute la fenêtre

	int largeur_fenetre = MLV_get_window_width();
	int hauteur_fenetre = MLV_get_window_height();

	MLV_Image *background = MLV_load_image("background.jpg");
	MLV_resize_image(background, largeur_fenetre, hauteur_fenetre);
	MLV_draw_image(background, 0, 0);

	MLV_actualise_window();

	return background;
}

void dessine_token(int x, int y, int taille, int couleur, int forme) {
	// Dessine un token dont le centre est la position (x, y)
	// Dessine également un contour noir autour du token

	MLV_Color mlv_couleur;

	switch(couleur) {
		case 0:	// Rouge
				mlv_couleur = MLV_COLOR_FIREBRICK4;
				break;
		case 1:	// Vert
				mlv_couleur = MLV_COLOR_CHARTREUSE4;
				break;
		case 2:	// Bleu
				mlv_couleur = MLV_COLOR_DODGERBLUE4;
				break;
		case 3:	// Jaune
				mlv_couleur = MLV_COLOR_DARKGOLDENROD3;
				break;
		default:
				// Invisible (noir transparent)
				mlv_couleur = MLV_rgba(0, 0, 0, 0);
				break;
	}

	switch(forme) {
		case 0: // Cercle
				MLV_draw_filled_circle(x, y, taille/2, mlv_couleur);
				MLV_draw_circle(x, y, taille/2, MLV_COLOR_BLACK);
				break;
		case 1: // Carré
				MLV_draw_filled_rectangle(x - taille/2, y - taille/2, taille, taille, mlv_couleur);
				MLV_draw_rectangle(x - taille/2, y - taille/2, taille, taille, MLV_COLOR_BLACK);
				break;
		case 2:	{
				// Triangle
				// Le premier point est le coin inférieur gauche du triangle, le second est le coin supérieur et le troisième est le coin inférieur droit.
				int triangle_x[3] = { x - taille/2, x, x + taille/2 };
				int triangle_y[3] = { y + taille/2, y - taille/2, y + taille/2 };
				MLV_draw_filled_polygon(triangle_x, triangle_y, 3, mlv_couleur);
				MLV_draw_polygon(triangle_x, triangle_y, 3, MLV_COLOR_BLACK);
				break;
				}
		case 3:	{
				// Diamant
				// Le premier point est celui du bas, le second celui de gauche, le troisième celui du haut et le quatrième celui de droite.
				int diamant_x[4] = { x, x - taille/2, x, x + taille/2 };
				int diamant_y[4] = { y + taille/2, y, y - taille/2, y };
				MLV_draw_filled_polygon(diamant_x, diamant_y, 4, mlv_couleur);
				MLV_draw_polygon(diamant_x, diamant_y, 4, MLV_COLOR_BLACK);
				break;
				}
	}

	MLV_actualise_window();
}

void dessine_suivant(MLV_Image *background, int couleur, int forme) {
	// Affiche un token dans le cadre suivant

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	int taille = 0, vitesse = 3, decalage_y = hauteur*0.08;

	while ( taille < largeur*0.06 ) {
		// Efface l'ancien token en superposant l'image de fond par dessus
		MLV_draw_partial_image(background, largeur*0.468, hauteur*0.11, largeur*0.065, hauteur*0.165, largeur*0.468, hauteur*0.11);
		dessine_token(largeur/2, hauteur*0.23 - decalage_y, taille, couleur, forme);

		if ( decalage_y > 0 )
			decalage_y -= 1;
		taille += 1;
		MLV_wait_milliseconds(vitesse);
	}
}

void dessine_liste(Zone **clic_tokens, MLV_Image *background, Liste lst) {
	// Dessine tous les tokens présent dans la liste de sorte à ce qu'ils soient centrés
	// Cette fonction peut dessiner au maximum 11 tokens, de plus, pour chaque tokens affiché,
	// la fonction enregistre une zone carré entourant le token.

	int i, compteur = 0;
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	// "Nettois la table", efface tous les tokens en superposant l'image de fond par dessus
	MLV_draw_partial_image(background, largeur*0.09, hauteur*0.43, largeur*0.82, hauteur*0.15, largeur*0.09, hauteur*0.43);

	// On crée un tableau qui contiendra les zones des tokens, dans lesquelles l'utilisateur cliquera pour faire les décalages circulaires
	free(*clic_tokens);
	*clic_tokens = (Zone*)malloc(sizeof(Zone) * len(lst) );

	for (i=0; i < 11; i++) {
		if ( i >= 5 - len(lst) / 2 && compteur < len(lst)) {
			dessine_token(largeur*0.126 + i*largeur*0.075, hauteur*0.492, largeur*0.06, (*lst).couleur, (*lst).forme);
			(*clic_tokens)[compteur].x1 = largeur*0.126 + i*largeur*0.075 - (largeur*0.06 / 2);
			(*clic_tokens)[compteur].y1 = hauteur*0.492 - (largeur*0.06 / 2);
			(*clic_tokens)[compteur].x2 = (*clic_tokens)[compteur].x1 + largeur*0.06 + 2;
			(*clic_tokens)[compteur].y2 = (*clic_tokens)[compteur].y1 + largeur*0.06 + 2;
			lst = (*lst).suivant;
			compteur++;
		}
	}

	MLV_actualise_window();
}

void cree_boutons_insertion(Zone *bouton_gauche, Zone *bouton_droit) {
	// Affiche les deux boutons d'insertion à gauche et à droite
	// et envois à bouton_gauche et bouton_droit les coordonnées
	// des rectangles des boutons.

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();
	MLV_Font *police;

	(*bouton_gauche).x1 = largeur*0.1;
	(*bouton_gauche).y1 = hauteur*0.07;
	(*bouton_gauche).x2 = (*bouton_gauche).x1 + largeur*0.1;
	(*bouton_gauche).y2 = (*bouton_gauche).y1 + hauteur*0.08;

	(*bouton_droit).x1 = largeur*0.8;
	(*bouton_droit).y1 = hauteur*0.07;
	(*bouton_droit).x2 = (*bouton_droit).x1 + largeur*0.1;
	(*bouton_droit).y2 = (*bouton_droit).y1 + hauteur*0.08;

	// Contour noir des cases
	MLV_draw_filled_rectangle((*bouton_gauche).x1, (*bouton_gauche).y1, (*bouton_gauche).x2 - (*bouton_gauche).x1, (*bouton_gauche).y2 - (*bouton_gauche).y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle((*bouton_droit).x1, (*bouton_droit).y1, (*bouton_droit).x2 - (*bouton_droit).x1, (*bouton_droit).y2 - (*bouton_droit).y1, MLV_COLOR_BLACK);

	// Cases
	MLV_draw_filled_rectangle((*bouton_gauche).x1 + 1, (*bouton_gauche).y1 + 1, (*bouton_gauche).x2 - (*bouton_gauche).x1 - 2, (*bouton_gauche).y2 - (*bouton_gauche).y1 - 2, MLV_COLOR_DARKSLATEGRAY4);
	MLV_draw_filled_rectangle((*bouton_droit).x1 + 1, (*bouton_droit).y1 + 1, (*bouton_droit).x2 - (*bouton_droit).x1 - 2, (*bouton_droit).y2 - (*bouton_droit).y1 - 2, MLV_COLOR_DARKSLATEGRAY4);

	// Texte "Insérer"
	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.025);
	MLV_draw_text_with_font((*bouton_gauche).x1 + largeur*0.007, (*bouton_gauche).y1 + hauteur*0.012, "INSERER", police, MLV_COLOR_DODGERBLUE4);
	MLV_draw_text_with_font((*bouton_droit).x1 + largeur*0.007, (*bouton_droit).y1 + hauteur*0.012, "INSERER", police, MLV_COLOR_DODGERBLUE4);
	MLV_free_font(police);

	MLV_actualise_window();
}

Zone dessine_decalage_couleur(Zone token, int couleur) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();
	Zone zone;

	dessine_token(token.x1 + (token.x2 - token.x1)/2, token.y1 - hauteur*0.01 - largeur*0.06 / 2, largeur*0.06, couleur, 1);

	zone.x1 = token.x1;
	zone.y1 = token.y1 - hauteur*0.01 - largeur*0.06;
	zone.x2 = token.x2;
	zone.y2 = zone.y1 + largeur*0.06;

	MLV_actualise_window();

	return zone;
}

Zone dessine_decalage_forme(Zone token, int forme) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();
	Zone zone;

	dessine_token(token.x1 + (token.x2 - token.x1)/2, token.y2 + hauteur*0.01 + largeur*0.06 / 2, largeur*0.06, -1, forme);

	zone.x1 = token.x1;
	zone.y1 = token.y2 + hauteur*0.01;
	zone.x2 = token.x2;
	zone.y2 = zone.y1 + largeur*0.06;

	MLV_actualise_window();

	return zone;
}

void efface_decalage_couleur(MLV_Image *background) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_draw_partial_image(background, largeur*0.09, hauteur*0.3, largeur*0.82, largeur*0.0752, largeur*0.09, hauteur*0.3);

	MLV_actualise_window();
}

void efface_decalage_forme(MLV_Image *background) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_draw_partial_image(background, largeur*0.09, hauteur*0.555, largeur*0.82, largeur*0.065, largeur*0.09, hauteur*0.555);

	MLV_actualise_window();
}

void concatene_string(char* premier, char* deuxieme) {
	// Concatene le chaine deuxieme à la suite de la chaine premier

	int i, j;

	for (i=0; premier[i] != '\0'; i++);

	for (j=0; deuxieme[j] != '\0'; j++) {
		premier[i] = deuxieme[j];
		i++;
	}

	premier[i] = '\0';
}

int len_str(char string[]) {
	int compteur = 0;

	while (string[compteur] != '\0') {
		compteur++;
	}

	return compteur;
}

void converti_int_en_char(char string[], int nombre) {
	int i = 0;
	char temp;

	do {
		string[i] = nombre % 10 + '0';
		nombre /= 10;
		i++;
	} while (nombre / 10 != 0);

	string[i] = nombre % 10 + '0';
	string[i+1] = '\0';

	/* On inverse la chaine car elle a été créé à l'envers */
	for (i=0; i < len_str(string)/2; i++) {
		temp = string[len_str(string)-1 -i];
		string[len_str(string)-1 -i] = string[i];
		string[i] = temp;
	}
}

void efface_temps(MLV_Image* background) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_draw_partial_image(background, largeur*0.176, hauteur*0.945, largeur*0.09, hauteur*0.045, largeur*0.176, hauteur*0.945);
}

int affiche_temps_restant(int debut, MLV_Image* background) {
	// Affiche le temps restant (sur une base de 2 minutes)  en bas à gauche de l'écran
	// 'debut' étant le nombre de millisecondes écoulées depuis le début du programme.
	// Renvois le nombre de secondes restantes.

	efface_temps(background);

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_Font* police;
	int delais = 120; // Nombre de secondes de jeu
	int temps_restant = delais*1000 - (MLV_get_time() - debut);
	int minute = ( temps_restant / 1000 ) / 60;
	int seconde = ( temps_restant / 1000 ) % 60;
	char minute_str[3], seconde_str[3];
	char text[30] = "Temps restant : ";

	converti_int_en_char(minute_str, minute);
	converti_int_en_char(seconde_str, seconde);
	concatene_string(text, minute_str);
	concatene_string(text, " : ");
	concatene_string(text, seconde_str);

	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.025);

	MLV_draw_text_with_font(largeur*0.01, hauteur*0.94, text, police, MLV_COLOR_STEELBLUE4);

	MLV_free_font(police);
	MLV_actualise_window();

	return temps_restant / 1000;
}

void affiche_menu(Zone *jouer, Zone *charger, Zone *options, Zone *quitter) {
	// Affiche un menu avec 4 choix, renvois par adresse les zones de clic de ces 4 choix.

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_Font *police;
	int width, height;

	MLV_clear_window(MLV_COLOR_LIGHT_CYAN);

	police = MLV_load_font("Comfortaa-Regular.ttf", largeur*0.1);
	MLV_get_size_of_text_with_font("Three to go !", &width, NULL, police);
	MLV_draw_text_with_font(largeur*0.5 - width*0.5, hauteur*0.08 , "Three to go !", police, MLV_COLOR_ROYALBLUE1);
	MLV_free_font(police);

	MLV_draw_line(largeur*0.1, hauteur*0.3, largeur*0.9, hauteur*0.3, MLV_COLOR_ROYALBLUE3);

	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.05);

	(*jouer).x1 = largeur*0.15;
	(*jouer).y1 = hauteur*0.45;
	(*jouer).x2 = (*jouer).x1 + largeur*0.3;
	(*jouer).y2 = (*jouer).y1 + hauteur*0.15;

	MLV_draw_rectangle((*jouer).x1, (*jouer).y1, (*jouer).x2 - (*jouer).x1, (*jouer).y2 - (*jouer).y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle((*jouer).x1 + 1, (*jouer).y1 + 1, (*jouer).x2 - (*jouer).x1 - 2, (*jouer).y2 - (*jouer).y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Jouer", &width, &height, police);
	MLV_draw_text_with_font((*jouer).x1 + ((*jouer).x2 - (*jouer).x1)*0.5 - width*0.5, (*jouer).y1 + ((*jouer).y2 - (*jouer).y1)*0.5 - height*0.55, "Jouer", police, MLV_COLOR_ROYALBLUE3);

	(*charger).x1 = largeur*0.55;
	(*charger).y1 = hauteur*0.45;
	(*charger).x2 = (*charger).x1 + largeur*0.3;
	(*charger).y2 = (*charger).y1 + hauteur*0.15;

	MLV_draw_rectangle((*charger).x1, (*charger).y1, (*charger).x2 - (*charger).x1, (*charger).y2 - (*charger).y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle((*charger).x1 + 1, (*charger).y1 + 1, (*charger).x2 - (*charger).x1 - 2, (*charger).y2 - (*charger).y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Charger", &width, &height, police);
	MLV_draw_text_with_font((*charger).x1 + ((*charger).x2 - (*charger).x1)*0.5 - width*0.5, (*charger).y1 + ((*charger).y2 - (*charger).y1)*0.5 - height*0.55, "Charger", police, MLV_COLOR_ROYALBLUE3);

	(*options).x1 = largeur*0.15;
	(*options).y1 = hauteur*0.7;
	(*options).x2 = (*options).x1 + largeur*0.3;
	(*options).y2 = (*options).y1 + hauteur*0.15;

	MLV_draw_rectangle((*options).x1, (*options).y1, (*options).x2 - (*options).x1, (*options).y2 - (*options).y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle((*options).x1 + 1, (*options).y1 + 1, (*options).x2 - (*options).x1 - 2, (*options).y2 - (*options).y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Options", &width, &height, police);
	MLV_draw_text_with_font((*options).x1 + ((*options).x2 - (*options).x1)*0.5 - width*0.5, (*options).y1 + ((*options).y2 - (*options).y1)*0.5 - height*0.55, "Options", police, MLV_COLOR_ROYALBLUE3);

	(*quitter).x1 = largeur*0.55;
	(*quitter).y1 = hauteur*0.7;
	(*quitter).x2 = (*quitter).x1 + largeur*0.3;
	(*quitter).y2 = (*quitter).y1 + hauteur*0.15;

	MLV_draw_rectangle((*quitter).x1, (*quitter).y1, (*quitter).x2 - (*quitter).x1, (*quitter).y2 - (*quitter).y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle((*quitter).x1 + 1, (*quitter).y1 + 1, (*quitter).x2 - (*quitter).x1 - 2, (*quitter).y2 - (*quitter).y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Quitter", &width, &height, police);
	MLV_draw_text_with_font((*quitter).x1 + ((*quitter).x2 - (*quitter).x1)*0.5 - width*0.5, (*quitter).y1 + ((*quitter).y2 - (*quitter).y1)*0.5 - height*0.55, "Quitter", police, MLV_COLOR_ROYALBLUE3);

	MLV_free_font(police);

	MLV_actualise_window();
}

int affiche_fin(int score) {
	// Affiche l'écran fin avec le score et 2 choix : Rejouer et quitter
	// La fonction renvois 1 si l'utilisateur clique sur rejouer et 0 si
	// l'utilisateur clique sur menu

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_clear_window(MLV_COLOR_LIGHT_CYAN);

	MLV_Font *police;
	int width, height;
	char score_char[10];
	char text[15] = "SCORE : ";
	Zone rejouer, menu;
	int clic_x, clic_y;

	police = MLV_load_font("Comfortaa-Regular.ttf", largeur*0.1);
	MLV_get_size_of_text_with_font("Game over", &width, NULL, police);
	MLV_draw_text_with_font(largeur*0.5 - width*0.5, hauteur*0.08 , "Game over", police, MLV_COLOR_ROYALBLUE1);
	MLV_free_font(police);

	MLV_draw_line(largeur*0.1, hauteur*0.3, largeur*0.9, hauteur*0.3, MLV_COLOR_ROYALBLUE3);

	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.05);

	converti_int_en_char(score_char, score);
	concatene_string(text, score_char);

	MLV_get_size_of_text_with_font(text, &width, NULL, police);
	MLV_draw_text_with_font(largeur*0.5 - width*0.5, hauteur*0.43, text, police, MLV_COLOR_ROYALBLUE3);

	rejouer.x1 = largeur*0.15;
	rejouer.y1 = hauteur*0.7;
	rejouer.x2 = rejouer.x1 + largeur*0.3;
	rejouer.y2 = rejouer.y1 + hauteur*0.15;

	MLV_draw_rectangle(rejouer.x1, rejouer.y1, rejouer.x2 - rejouer.x1, rejouer.y2 - rejouer.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(rejouer.x1 + 1, rejouer.y1 + 1, rejouer.x2 - rejouer.x1 - 2, rejouer.y2 - rejouer.y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Rejouer", &width, &height, police);
	MLV_draw_text_with_font(rejouer.x1 + (rejouer.x2 - rejouer.x1)*0.5 - width*0.5, rejouer.y1 + (rejouer.y2 - rejouer.y1)*0.5 - height*0.55, "Rejouer", police, MLV_COLOR_ROYALBLUE3);

	menu.x1 = largeur*0.55;
	menu.y1 = hauteur*0.7;
	menu.x2 = menu.x1 + largeur*0.3;
	menu.y2 = menu.y1 + hauteur*0.15;

	MLV_draw_rectangle(menu.x1, menu.y1, menu.x2 - menu.x1, menu.y2 - menu.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(menu.x1 + 1, menu.y1 + 1, menu.x2 - menu.x1 - 2, menu.y2 - menu.y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Menu", &width, &height, police);
	MLV_draw_text_with_font(menu.x1 + (menu.x2 - menu.x1)*0.5 - width*0.5, menu.y1 + (menu.y2 - menu.y1)*0.5 - height*0.55, "Menu", police, MLV_COLOR_ROYALBLUE3);

	MLV_free_font(police);

	MLV_actualise_window();

	while ( 1 ) {
		MLV_wait_mouse(&clic_x, &clic_y);

		if (rejouer.x1 < clic_x && clic_x < rejouer.x2 && rejouer.y1 < clic_y && clic_y < rejouer.y2)
			return 1;
		else if (menu.x1 < clic_x && clic_x < menu.x2 && menu.y1 < clic_y && clic_y < menu.y2)
			return 0;
	}
}

int abs(x) {
	// Retourne la valeur absolue de x
	if (x > 0)
		return x;
	else
		return -x;
}

void affiche_options() {
	// Affiche les options proposées

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_clear_window(MLV_rgba(0 ,0 ,0 ,180));

	MLV_Font* police;
	Zone resolution, gauche, droite, plein_ecran;
	int width, height;
	int pgx[3] = { largeur*0.32, largeur*0.28, largeur*0.32 };
	int pgy[3] = { hauteur*0.38, hauteur*0.425, hauteur*0.47 };
	int pdx[3] = { largeur*0.68, largeur*0.72, largeur*0.68 };
	int pdy[3] = { hauteur*0.38, hauteur*0.425, hauteur*0.47 };
	int clic_x, clic_y;
	int i;
	char text_resolution[3][15] = { "1280 * 720", "1920 * 1080", "640 * 360" };

	MLV_draw_rectangle(largeur*0.2, hauteur*0.2, largeur*0.6, hauteur*0.6, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(largeur*0.2 + 1, hauteur*0.2 + 1, largeur*0.6 - 2, hauteur*0.6 - 2, MLV_COLOR_LIGHT_CYAN);

	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.04);
	MLV_draw_text_with_font(largeur*0.25, hauteur*0.25, "Résolution", police, MLV_COLOR_ROYALBLUE2);

	// Cadre résolution

	resolution.x1 = largeur*0.25;
	resolution.y1 = hauteur*0.35;
	resolution.x2 = resolution.x1 + largeur*0.5;
	resolution.y2 = resolution.y1 + hauteur*0.15;

	MLV_draw_rectangle(resolution.x1, resolution.y1, resolution.x2 - resolution.x1, resolution.y2 - resolution.y1, MLV_COLOR_BLACK);

	// Flèche gauche

	gauche.x1 = largeur*0.25;
	gauche.y1 = hauteur*0.35;
	gauche.x2 = gauche.x1 + largeur*0.1;
	gauche.y2 = gauche.y1 + hauteur*0.15;

	MLV_draw_rectangle(gauche.x1, gauche.y1, gauche.x2 - gauche.x1, gauche.y2 - gauche.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(gauche.x1 + 1, gauche.y1 + 1, gauche.x2 - gauche.x1 - 2, gauche.y2 - gauche.y1 - 2, MLV_COLOR_PALETURQUOISE);

	MLV_draw_polygon(pgx, pgy, 3, MLV_COLOR_BLACK);

	// Flèche droite

	droite.x1 = largeur*0.65;
	droite.y1 = hauteur*0.35;
	droite.x2 = droite.x1 + largeur*0.1;
	droite.y2 = droite.y1 + hauteur*0.15;

	MLV_draw_rectangle(droite.x1, droite.y1, droite.x2 - droite.x1, droite.y2 - droite.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(droite.x1 + 1, droite.y1 + 1, droite.x2 - droite.x1 - 2, droite.y2 - droite.y1 - 2, MLV_COLOR_PALETURQUOISE);

	MLV_draw_polygon(pdx, pdy, 3, MLV_COLOR_BLACK);

	// Cadre plein écran

	plein_ecran.x1 = largeur*0.25;
	plein_ecran.y1 = hauteur*0.55;
	plein_ecran.x2 = plein_ecran.x1 + largeur*0.5;
	plein_ecran.y2 = plein_ecran.y1 + hauteur*0.15;

	MLV_draw_rectangle(plein_ecran.x1, plein_ecran.y1, plein_ecran.x2 - plein_ecran.x1, plein_ecran.y2 - plein_ecran.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(plein_ecran.x1 + 1, plein_ecran.y1 + 1, plein_ecran.x2 - plein_ecran.x1 - 2, plein_ecran.y2 - plein_ecran.y1 - 2, MLV_COLOR_PALETURQUOISE);

	if ( !MLV_is_full_screen() ) {
		MLV_get_size_of_text_with_font("Plein écran", &width, &height, police);
		MLV_draw_text_with_font(plein_ecran.x1 + (plein_ecran.x2 - plein_ecran.x1)*0.5 - width*0.5, plein_ecran.y1 + (plein_ecran.y2 - plein_ecran.y1)*0.5 - height*0.55, "Plein écran", police, MLV_COLOR_ROYALBLUE2);
	} else {
		MLV_get_size_of_text_with_font("Désactiver plein écran", &width, &height, police);
		MLV_draw_text_with_font(plein_ecran.x1 + (plein_ecran.x2 - plein_ecran.x1)*0.5 - width*0.5, plein_ecran.y1 + (plein_ecran.y2 - plein_ecran.y1)*0.5 - height*0.55, "Désactiver plein écran", police, MLV_COLOR_ROYALBLUE2);
	}

	i = 0;
	do {
		// Efface le dernier texte
		MLV_draw_filled_rectangle(gauche.x2, gauche.y1 + 1, droite.x1 - gauche.x2, droite.y2 - gauche.y1 - 2, MLV_COLOR_LIGHT_CYAN);
		// Affiche le texte
		MLV_get_size_of_text_with_font(text_resolution[i], &width, &height, police);
		MLV_draw_text_with_font(resolution.x1 + (resolution.x2 - resolution.x1)*0.5 - width*0.5, resolution.y1 + (resolution.y2 - resolution.y1)*0.21, text_resolution[i], police, MLV_COLOR_ROYALBLUE2);
		MLV_actualise_window();

		MLV_wait_mouse(&clic_x, &clic_y);

		if (gauche.x1 < clic_x && clic_x < gauche.x2 && gauche.y1 < clic_y && clic_y < gauche.y2)
			// Flèche gauche
			if ( i == 0)
				i = 2;
			else
				i = abs(i - 1)%3;
		else if (droite.x1 < clic_x && clic_x < droite.x2 && droite.y1 < clic_y && clic_y < droite.y2)
			// Flèche droite
			i = abs(i + 1)%3;
		else if (gauche.x2 < clic_x && clic_x < droite.x1 && gauche.y1 < clic_y && clic_y < droite.y2) {
			// Cadre de résolution
			if ( i == 0 ) {
				MLV_change_window_size(1280, 720);
			} else if ( i == 1 ) {
				MLV_change_window_size(1920, 1080);
			} else if ( i == 2 ) {
				MLV_change_window_size(640, 360);
			}
			break;
		} else if (plein_ecran.x1 < clic_x && clic_x < plein_ecran.x2 && plein_ecran.y1 < clic_y && clic_y < plein_ecran.y2) {
			// Plein écran
			if ( MLV_is_full_screen() ) {
				MLV_disable_full_screen();
			} else {
				MLV_enable_full_screen();
			}
			break;
		} else if ( !(largeur*0.2 < clic_x && clic_x < largeur*0.2 + largeur*0.6 && hauteur*0.2 < clic_y && clic_y < hauteur*0.2 + hauteur*0.6) ) {
			// Si on clique en dehors du cadre
			break;
		}
	} while ( 1 );

	MLV_free_font(police);
}

void affiche_score(int score, MLV_Image* background) {
	// Affiche le score en bas à droite

	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_draw_partial_image(background, largeur*0.7, hauteur*0.93, largeur*0.3, hauteur*0.07, largeur*0.7, hauteur*0.93);

	int width;
	char score_char[5] = "";
	char texte[15] = "Score : ";
	MLV_Font * police;

	converti_int_en_char(score_char, score);
	concatene_string(texte, score_char);

	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.025);

	MLV_get_size_of_text_with_font(texte, &width, NULL, police);
	MLV_draw_text_with_font(largeur - width*1.1, hauteur*0.94, texte, police, MLV_COLOR_STEELBLUE4);

	MLV_free_font(police);
	MLV_actualise_window();
}

void disparait_token(MLV_Image* background, Zone* zone_tokens, Liste lst, int nb_tokens, ...) {
	int largeur = MLV_get_window_width();

	int i;
	int x1, y1, x2, y2;
	int vitesse, taille = largeur*0.06;
	va_list ap;

	Token* tab_tokens[5];

	va_start(ap, nb_tokens);
	for (i=0; i < nb_tokens; i++) {
		tab_tokens[i] = va_arg(ap, Token*);
	}
	va_end(ap);

	// Baisser la vitesse permet en faite de l'augmenter
	vitesse = 15;

	while ( taille > 0 ) {
		for (i=0; i < nb_tokens; i++) {
			x1 = zone_tokens[indice_token(lst, tab_tokens[i])].x1;
			y1 = zone_tokens[indice_token(lst, tab_tokens[i])].y1;
			x2 = zone_tokens[indice_token(lst, tab_tokens[i])].x2;
			y2 = zone_tokens[indice_token(lst, tab_tokens[i])].y2;

			MLV_draw_partial_image(background, x1, y1, x2 - x1, y2 - y1, x1, y1);
			taille -= 1;
			dessine_token(x1 + (x2 - x1)*0.5, y1 + (y2 - y1)*0.5, taille, (*tab_tokens[i]).couleur, (*tab_tokens[i]).forme);
		}
		MLV_wait_milliseconds(vitesse);
	}

	MLV_actualise_window();
}

void disparait_suivant(MLV_Image* background, int couleur, int forme) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	int vitesse, taille = largeur*0.06;
	int decalage_y = 1;

	vitesse = 3;
	while ( taille > 0 ) {
		MLV_draw_partial_image(background, largeur*0.468, hauteur*0.175, largeur*0.065, hauteur*0.18, largeur*0.468, hauteur*0.175);
		dessine_token(largeur/2, hauteur*0.23 + decalage_y, taille, couleur, forme);
		
		taille -= 2;
		decalage_y += 1;
		MLV_wait_milliseconds(vitesse);
	}

	MLV_draw_partial_image(background, largeur*0.468, hauteur*0.175, largeur*0.065, hauteur*0.18, largeur*0.468, hauteur*0.175);
}

void affiche_bouton_pause(Zone *pause) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	int width, height;
	MLV_Font *police;

	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.025);

	MLV_get_size_of_text_with_font("Pause", &width, &height, police);

	(*pause).x1 = largeur*0.5 - width*0.6;
	(*pause).y1 = hauteur*0.94;
	(*pause).x2 = (*pause).x1 + width*1.2;
	(*pause).y2 = (*pause).y1 + height*1.1;

	MLV_draw_rectangle((*pause).x1, (*pause).y1, (*pause).x2 - (*pause).x1, (*pause).y2 - (*pause).y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle((*pause).x1 + 1, (*pause).y1 + 1, (*pause).x2 - (*pause).x1 - 2, (*pause).y2 - (*pause).y1 - 2, MLV_COLOR_STEEL_BLUE);

	MLV_draw_text_with_font(largeur*0.5 - width*0.5, hauteur*0.94, "Pause", police, MLV_COLOR_DODGERBLUE4);

	MLV_actualise_window();

	MLV_free_font(police);
}

int affiche_pause(int* temps_depart) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	int debut = MLV_get_time();

	MLV_Font *police;
	Zone pause, continuer, sauvegarder, terminer;
	int width, height;
	int clic_x, clic_y;

	// On enregistre l'écran pour le réafficher après avoir quitté le menu pause
	MLV_save_screen();

	MLV_clear_window(MLV_rgba(0, 0, 0, 180));

	pause.x1 = largeur*0.1;
	pause.y1 = hauteur*0.1;
	pause.x2 = pause.x1 + largeur*0.8;
	pause.y2 = pause.y1 + hauteur*0.8;

	MLV_draw_filled_rectangle(pause.x1, pause.y1, pause.x2 - pause.x1, pause.y2 - pause.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(pause.x1 + 5, pause.y1 + 5, pause.x2 - pause.x1 - 10, pause.y2 - pause.y1 - 10, MLV_COLOR_LIGHT_CYAN);

	police = MLV_load_font("Comfortaa-Regular.ttf", largeur*0.08);

	MLV_get_size_of_text_with_font("Pause", &width, NULL, police);
	MLV_draw_text_with_font(largeur*0.5 - width*0.5, pause.y1*1.3, "Pause", police, MLV_COLOR_ROYALBLUE1);

	MLV_free_font(police);
	police = MLV_load_font("coolvetica_rg.ttf", largeur*0.04);

	MLV_draw_line(largeur*0.2, hauteur*0.3, largeur*0.8, hauteur*0.3, MLV_COLOR_ROYALBLUE3);

	continuer.x1 = largeur*0.3;
	continuer.y1 = hauteur*0.4;
	continuer.x2 = continuer.x1 + largeur*0.4;
	continuer.y2 = continuer.y1 + hauteur*0.1;

	MLV_draw_rectangle(continuer.x1, continuer.y1, continuer.x2 - continuer.x1, continuer.y2 - continuer.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(continuer.x1 + 1, continuer.y1 + 1, continuer.x2 - continuer.x1 - 2, continuer.y2 - continuer.y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Continuer", &width, &height, police);
	MLV_draw_text_with_font(continuer.x1 + (continuer.x2 - continuer.x1)*0.5 - width*0.5, continuer.y1 + (continuer.y2 - continuer.y1)*0.5 - height*0.55, "Continuer", police, MLV_COLOR_ROYALBLUE3);

	sauvegarder.x1 = largeur*0.3;
	sauvegarder.y1 = hauteur*0.55;
	sauvegarder.x2 = sauvegarder.x1 + largeur*0.4;
	sauvegarder.y2 = sauvegarder.y1 + hauteur*0.1;

	MLV_draw_rectangle(sauvegarder.x1, sauvegarder.y1, sauvegarder.x2 - sauvegarder.x1, sauvegarder.y2 - sauvegarder.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(sauvegarder.x1 + 1, sauvegarder.y1 + 1, sauvegarder.x2 - sauvegarder.x1 - 2, sauvegarder.y2 - sauvegarder.y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Sauvegarder", &width, &height, police);
	MLV_draw_text_with_font(sauvegarder.x1 + (sauvegarder.x2 - sauvegarder.x1)*0.5 - width*0.5, sauvegarder.y1 + (sauvegarder.y2 - sauvegarder.y1)*0.5 - height*0.55, "Sauvegarder", police, MLV_COLOR_ROYALBLUE3);

	terminer.x1 = largeur*0.3;
	terminer.y1 = hauteur*0.7;
	terminer.x2 = terminer.x1 + largeur*0.4;
	terminer.y2 = terminer.y1 + hauteur*0.1;

	MLV_draw_rectangle(terminer.x1, terminer.y1, terminer.x2 - terminer.x1, terminer.y2 - terminer.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(terminer.x1 + 1, terminer.y1 + 1, terminer.x2 - terminer.x1 - 2, terminer.y2 - terminer.y1 - 2, MLV_COLOR_PALETURQUOISE);
	MLV_get_size_of_text_with_font("Terminer la partie", &width, &height, police);
	MLV_draw_text_with_font(terminer.x1 + (terminer.x2 - terminer.x1)*0.5 - width*0.5, terminer.y1 + (terminer.y2 - terminer.y1)*0.5 - height*0.55, "Terminer la partie", police, MLV_COLOR_ROYALBLUE3);

	MLV_actualise_window();
	MLV_free_font(police);

	do  {
		MLV_wait_mouse(&clic_x, &clic_y);

		if ( continuer.x1 < clic_x && clic_x < continuer.x2 && continuer.y1 < clic_y && clic_y < continuer.y2 ) {
			*temps_depart += MLV_get_time() - debut;
			MLV_load_screen();
			return 0;
		} else if ( sauvegarder.x1 < clic_x && clic_x < sauvegarder.x2 && sauvegarder.y1 < clic_y && clic_y < sauvegarder.y2 ) {
			*temps_depart += MLV_get_time() - debut;
			MLV_load_screen();
			return 1;
		} else if ( terminer.x1 < clic_x && clic_x < terminer.x2 && terminer.y1 < clic_y && clic_y < terminer.y2 ) {
			*temps_depart += MLV_get_time() - debut;
			MLV_load_screen();
			return 2;
		}
	} while ( 1 );
}

void affiche_erreur_charger() {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_Font *police = MLV_load_font("coolvetica_rg.ttf", largeur*0.04);
	int width, height;
	int clic_x, clic_y;
	Zone ok;

	MLV_clear_window(MLV_rgba(0, 0, 0, 180));

	MLV_draw_rectangle(largeur*0.2, hauteur*0.25, largeur*0.6, hauteur*0.4, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(largeur*0.2 + 1, hauteur*0.25 + 1, largeur*0.6 - 2, hauteur*0.4 - 2, MLV_COLOR_LIGHT_CYAN);

	MLV_get_size_of_text_with_font("Aucune sauvegarde trouvée", &width, NULL, police);
	MLV_draw_text_with_font(largeur*0.5 - width*0.5, hauteur*0.3, "Aucune sauvegarde trouvée", police, MLV_COLOR_ROYALBLUE3);

	MLV_get_size_of_text_with_font("OK", &width, &height, police);

	ok.x1 = largeur*0.5 - width;
	ok.y1 = hauteur*0.5 - height*0.5;
	ok.x2 = ok.x1 + width*2;
	ok.y2 = ok.y1 + height;

	MLV_draw_rectangle(ok.x1, ok.y1, ok.x2 - ok.x1, ok.y2 - ok.y1, MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(ok.x1 + 1, ok.y1 + 1, ok.x2 - ok.x1 - 2, ok.y2 - ok.y1 - 2, MLV_COLOR_PALETURQUOISE);

	MLV_draw_text_with_font(ok.x1 + (ok.x2 - ok.x1)*0.5 - width*0.5, ok.y1*0.999, "OK", police, MLV_COLOR_ROYALBLUE3);

	MLV_actualise_window();

	do {
		MLV_wait_mouse(&clic_x, &clic_y);
	}	while ( !(ok.x1 < clic_x && clic_x < ok.x2 && ok.y1 < clic_y && clic_y < ok.y2) );
}

void ajouter_a_gauche_animation(Liste lst, int couleur, int forme) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_Image *background = MLV_load_image("background.jpg");
	MLV_resize_image(background, largeur, hauteur);

	int i, compteur = 0;
	int depart_x = largeur*0.095, depart_y = hauteur*0.235;
	int arrive_x = 0, arrive_y = 0;
	int x = depart_x, y = depart_y;
	float a, b; // Droite ax+b
	float taille = 0;
	float vitesse, distance, temps;

	// On repère la position du prochain token
	for (i=0; i < 11; i++) {
		if ( i >= 5 - (len(lst) + 1) / 2 && compteur < (len(lst) + 1)) {
			arrive_x = largeur*0.126 + i*largeur*0.075;
			arrive_y = hauteur*0.492;
			break;
		}
	}

	a = (float) (depart_y - arrive_y) / (depart_x - arrive_x);
	b = (float) depart_y - (a * depart_x);

	distance = sqrt( pow((arrive_y - depart_y), 2) + pow((arrive_x - depart_x), 2) );
	vitesse = 1;
	temps = distance / vitesse; // En milli seconde

	while ( x <= arrive_x ) {
		MLV_draw_partial_image(background, x - largeur*0.06 * 0.7, y - largeur*0.06 * 0.7, largeur*0.08, largeur*0.08, x - largeur*0.06 * 0.7, y - largeur*0.06 * 0.7);
		dessine_token(x, y, taille, couleur, forme);
		x++;
		y = a * x + b;
		if ( taille < largeur*0.06 )
			taille += distance / temps;
		while ( taille > largeur*0.06 )
			taille--;
		MLV_actualise_window();
	}

	MLV_free_image(background);
}

void ajouter_a_droite_animation(Liste lst, int couleur, int forme) {
	int hauteur = MLV_get_window_height();
	int largeur = MLV_get_window_width();

	MLV_Image *background = MLV_load_image("background.jpg");
	MLV_resize_image(background, largeur, hauteur);

	int i, compteur = 0;
	int depart_x = largeur*0.905, depart_y = hauteur*0.235;
	int arrive_x = 0, arrive_y = 0;
	int x = depart_x, y = depart_y;
	float a, b; // Droite ax+b
	float taille = 0;
	float vitesse, distance, temps;

	// On repère la position du prochain token
	for (i=0; i < 11; i++) {
		if ( i >= 5 - (len(lst) + 1) / 2 && compteur < (len(lst) + 1)) {
			arrive_x = largeur*0.126 + i*largeur*0.075;
			arrive_y = hauteur*0.492;
			compteur++;
		}
	}

	a = (float) (depart_y - arrive_y) / (depart_x - arrive_x);
	b = (float) depart_y - (a * depart_x);

	distance = sqrt( pow((arrive_y - depart_y), 2) + pow((arrive_x - depart_x), 2) );
	vitesse = 1;
	temps = distance / vitesse; // En milli seconde

	while ( x >= arrive_x ) {
		MLV_draw_partial_image(background, x - largeur*0.06 * 0.7, y - largeur*0.06 * 0.7, largeur*0.08, largeur*0.08, x - largeur*0.06 * 0.7, y - largeur*0.06 * 0.7);
		dessine_token(x, y, taille, couleur, forme);
		x--;
		y = a * x + b;
		if ( taille < largeur*0.06 )
			taille += distance / temps;
		while ( taille > largeur*0.06 )
			taille--;
		MLV_actualise_window();
	}

	MLV_free_image(background);
}

void circu_token(MLV_Image* background,Liste lst, Zone* zone_tokens, int type_circu, int valeur){
	int largeur = MLV_get_window_width();
	/*type_circu=0 on tourne selon la couleur*/
	/*type_circu=1 on tourne selon la forme*/
	/*valeur correspond à la valeur de la couleur ou de la forme*/
	Liste tmp=lst;
	Token* tab_tokens[11];
	int tab_test[11];
	int i= 0, j=0, etape=1,test=0;
	float test_hauteur=0, test_largeur=0;
	float taille = largeur*0.06;
	int x1, y1, x2, y2;
	efface_decalage_couleur(background);
	efface_decalage_forme(background);
	if (type_circu==0){
		while (tmp->suivant!=lst){
			if (tmp->couleur==valeur){
				tab_tokens[i]=tmp;
				tab_test[i]=0;
				i++;
			}
			tmp=tmp->suivant;
		}
		if (tmp->couleur==valeur){
			tab_tokens[i]=tmp;
			tab_test[i]=0;
			i++;
		}
	}else{
		while (tmp->suivant!=lst){
			if (tmp->forme==valeur){
				tab_tokens[i]=tmp;
				tab_test[i]=0;
				i++;
			}
			tmp=tmp->suivant;
		}
		if (tmp->forme==valeur){
			tab_tokens[i]=tmp;
			tab_test[i]=0;
			i++;
		}
	}
	while(j<i){
		j++;
	}
	j=0;
	while(etape<=3){
		if (etape==1){
			//Levée des tokens qui tournent
			while(test_hauteur<0.06*largeur){
				while(j<i){
					x1 = zone_tokens[indice_token(lst, tab_tokens[j])].x1;
					y1 = zone_tokens[indice_token(lst, tab_tokens[j])].y1;
					x2 = zone_tokens[indice_token(lst, tab_tokens[j])].x2;
					y2 = zone_tokens[indice_token(lst, tab_tokens[j])].y2;
					if (j==0){
						y1+=test_hauteur;
						y2+=test_hauteur;
					}else{
						y1-=test_hauteur;
						y2-=test_hauteur;
					}
					dessine_token(x1 + (x2 - x1)*0.5, y1 + (y2 - y1)*0.5, taille, (*tab_tokens[j]).couleur, (*tab_tokens[j]).forme);
					MLV_draw_partial_image(background, x1, y1, x2 - x1, y2 - y1, x1, y1);
					
					j++;
				}
				test_hauteur=test_hauteur+0.001*largeur;
				MLV_actualise_window();
				j=0;
			}
			etape=2;
		}
		if (etape==2){
			//Décalage des tokens
			while (test==0){
				test=1;
				while(j<i){
					if (tab_test[j]==0){
						test=0;
						x1 = zone_tokens[indice_token(lst, tab_tokens[j])].x1;
						y1 = zone_tokens[indice_token(lst, tab_tokens[j])].y1;
						x2 = zone_tokens[indice_token(lst, tab_tokens[j])].x2;
						y2 = zone_tokens[indice_token(lst, tab_tokens[j])].y2;
						if ( ((j==0 && x1<zone_tokens[indice_token(lst, tab_tokens[i-1])].x1)) || ((j!=0) && (x1<zone_tokens[indice_token(lst, tab_tokens[j-1])].x1)) ){
							x1+=test_largeur;
							x2+=test_largeur;
							if (j==0){
								y1+=test_hauteur;
								y2+=test_hauteur;
							}else{
								y1-=test_hauteur;
								y2-=test_hauteur;
							}
							MLV_draw_partial_image(background, x1-1, y1, x2+1 - x1, y2 - y1, x1-1, y1);
							dessine_token(x1 + (x2 - x1)*0.5, y1 + (y2 - y1)*0.5, taille, (*tab_tokens[j]).couleur, (*tab_tokens[j]).forme);
						}else{
							x1-=test_largeur;
							x2-=test_largeur;
							if (j==0){
								y1+=test_hauteur;
								y2+=test_hauteur;
							}else{
								y1-=test_hauteur;
								y2-=test_hauteur;
							}
							MLV_draw_partial_image(background, x1-1, y1, x2+1 - x1, y2 - y1, x1, y1);
							dessine_token(x1 + (x2 - x1)*0.5, y1 + (y2 - y1)*0.5, taille, (*tab_tokens[j]).couleur, (*tab_tokens[j]).forme);
						}
						
						if( ((j==0) && (x1==zone_tokens[indice_token(lst, tab_tokens[i-1])].x1)) || ((j!=0) && (x1==zone_tokens[indice_token(lst, tab_tokens[j-1])].x1)) ){
							tab_test[j]=1;
						}
					}
					j++;
				}
				test_largeur+=1;
				MLV_actualise_window();
				j=0;
			}
			etape=3;
		}
		if (etape==3){
			//descente des tokens
			while(test_hauteur>0){
				while(j<i){
					if (j!=0){
						x1 = zone_tokens[indice_token(lst, tab_tokens[j-1])].x1;
						y1 = zone_tokens[indice_token(lst, tab_tokens[j-1])].y1;
						x2 = zone_tokens[indice_token(lst, tab_tokens[j-1])].x2;
						y2 = zone_tokens[indice_token(lst, tab_tokens[j-1])].y2;
						y1-=test_hauteur;
						y2-=test_hauteur;
						MLV_draw_partial_image(background, x1, y1-0.001*largeur, x2 - x1, y2+0.001*largeur - y1, x1, y1-0.001*largeur);
						dessine_token(x1 + (x2 - x1)*0.5, y1 + (y2 - y1)*0.5, taille, (*tab_tokens[j]).couleur, (*tab_tokens[j]).forme);
					}else{
						x1 = zone_tokens[indice_token(lst, tab_tokens[i-1])].x1;
						y1 = zone_tokens[indice_token(lst, tab_tokens[i-1])].y1;
						x2 = zone_tokens[indice_token(lst, tab_tokens[i-1])].x2;
						y2 = zone_tokens[indice_token(lst, tab_tokens[i-1])].y2;
						y1+=test_hauteur;
						y2+=test_hauteur;
						MLV_draw_partial_image(background, x1, y1+0.001*largeur, x2 - x1, y2+0.001*largeur - y1, x1, y1+0.001*largeur);
						dessine_token(x1 + (x2 - x1)*0.5, y1 + (y2 - y1)*0.5, taille, (*tab_tokens[j]).couleur, (*tab_tokens[j]).forme);
					}

					j++;
				}
				test_hauteur-=0.001*largeur;
				MLV_actualise_window();
				j=0;
			}
			etape=4;
		}

	}
}