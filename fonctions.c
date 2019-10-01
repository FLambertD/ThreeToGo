void ajouter_a_gauche_animation(Liste lst, int couleur, int forme);
void ajouter_a_droite_animation(Liste lst, int couleur, int forme);

Token* allouer_token(int couleur, int forme) {
	// Alloue la place nécéssaire pour créer un Token,
	// puis initialise sa couleur et sa forme en fonction
	// des paramètres. Return NULL si l'allocation a échouée.

	Token *token = (Token*)malloc(sizeof(Token));
	if (token == NULL) {
		printf("L'allocation d'un token a échoué\n");
		return NULL;
	}

	// Liste chainée circulaire, donc le précédent et le suivant
	// d'un élément d'une liste composé d'un élément est lui même
	(*token).couleur = couleur;
	(*token).forme = forme;
	(*token).suivant = token;
	(*token).suivant_couleur = token;
	(*token).precedent_couleur = token;
	(*token).suivant_forme = token;
	(*token).precedent_forme = token;

	return token;
}

void affiche_liste(Liste lst) {
	// Affiche le tuple (couleur, forme) du contenu
	// de la liste lst de gauche à droite

	if (lst == NULL) {
		printf("Liste vide\n");
		return;
	}

	Liste tmp = lst;

	do {
		printf("(%d, %d)\t", (*lst).couleur, (*lst).forme);
		lst = (*lst).suivant;
	} while (lst != tmp);

	printf("\n");
}

void affiche_token(Token token) {
	// Affiche le tuple (couleur, forme) d'un token
	printf("(%d, %d)\n", token.couleur, token.forme);
}

int len(Liste lst) {
	// Renvois le nombre d'élément
	// que contient la liste lst

	if (lst == NULL)
		return 0;

	int compteur = 1;

	Liste debut = lst;
	lst = (*lst).suivant;

	while (lst != debut) {
		compteur++;
		lst = (*lst).suivant;
	}

	return compteur;
}

int len_couleur(Liste lst, int couleur) {
	// Renvois le nombre de token ayant la couleur
	// donnée en paramètre dans la liste

	if ( lst == NULL )
		return 0;

	Token *token;
	Token *premier_token;

	// On cherche le premier token qui a la couleur donnée en paramètre
	if ( (*lst).couleur != couleur ) {
		Liste debut = lst;
		lst = (*lst).suivant;

		for (; (*lst).couleur != couleur && lst != debut; lst = (*lst).suivant);

		if ( lst == debut)
			return 0;
	}

	token = lst;
	premier_token = token;
	int compteur = 0;

	do {
		compteur++;
		token = (*token).suivant_couleur;
	} while (token != premier_token);

	return compteur;
}

int len_forme(Liste lst, int forme) {
	// Renvois le nombre de token ayant la forme
	// donnée en paramètre dans la liste

	if ( lst == NULL )
		return 0;

	Token *token;
	Token *premier_token;

	// On cherche le premier token qui a la forme donnée en paramètre
	if ( (*lst).forme != forme ) {
		Liste debut = lst;
		lst = (*lst).suivant;

		for (; (*lst).forme != forme && lst != debut; lst = (*lst).suivant);

		if ( lst == debut)
			return 0;
	}

	token = lst;
	premier_token = token;
	int compteur = 0;

	do {
		compteur++;
		token = (*token).suivant_forme;
	} while (token != premier_token);

	return compteur;
}

Token* token_indice(Liste lst, int n) {
	// Retourne le token située à l'indice n

	int i = 0;

	do {
		if (i == n)
			return lst;
		lst = (*lst).suivant;
		i++;
	} while ( i < len(lst) );

	return NULL;
}

int indice_token(Liste lst, Token* token) {
	// Renvois l'indice du token dans la liste

	if ( lst == NULL )
		return -1;

	int i = 0;
	Liste debut = lst;

	do {
		if ( lst == token )
			return i;
		lst = (*lst).suivant;
		i++;
	} while ( lst != debut );

	// Le token n'est pas présent dans la liste
	return -1;
}

Token* suivant_couleur(Token *token) {
	// Parcours la liste de gauche à droite jusqu'à
	// trouver un token avec la même couleur que celui
	// donné en paramètre

	Token *tmp = token;

	do {
		tmp = (*tmp).suivant;
	} while ( (*tmp).couleur != (*token).couleur );

	// On est sur que tmp ne vaudra pas nul car s'il n'y a
	// pas d'autre token avec la même couleur, le suivant sera
	// lui même
	return tmp;
}

Token* suivant_forme(Token *token) {
	// Suit le même fonctionnement que
	// suivant_couleur() sauf que la boucle
	// s'arrête lorsqu'elle trouve un tokken
	// avec la même forme

	Token *tmp = token;

	do {
		tmp = (*tmp).suivant;
	} while ( (*tmp).forme != (*token).forme );

	return tmp;
}

Token* precedent_couleur(Token *token) {
	// Renvois le dernier token ayant la même couleur que
	// celui passé en paramètre. On parcours la liste de gauche
	// à droite et on retient à chaque fois le dernier token ayant
	// la même couleur jusqu'à ce qu'on revienne sur nous même

	Token *tmp = token;
	Token *precedent = token;

	do {
		if ( (*tmp).couleur == (*token).couleur )
			precedent = tmp;
		tmp = (*tmp).suivant;
	} while (tmp != token);

	return precedent;
}

Token* precedent_forme(Token *token) {
	// Pareil que precedent_couleur() mais avec les formes.

	Token *tmp = token;
	Token *precedent = token;

	do {
		if ( (*tmp).forme == (*token).forme )
			precedent = tmp;
		tmp = (*tmp).suivant;
	} while (tmp != token);

	return precedent;
}

void actualise_chainage(Liste lst) {
	// A chaque fois qu'on modifie la liste, la
	// forme et la couleur précédente et suivante
	// de chaque token doivent également être modifiées

	if ( lst == NULL ) {
		return;
	}

	Liste debut = lst;

	do {
		(*lst).suivant_couleur = suivant_couleur(lst);
		(*lst).precedent_couleur = precedent_couleur(lst);
		(*lst).suivant_forme = suivant_forme(lst);
		(*lst).precedent_forme = precedent_forme(lst);
		lst = (*lst).suivant;
	} while ( lst != debut );
}

int ajouter_a_gauche(Liste *lst, int couleur, int forme, int animation) {
	// Ajoute un token de couleur et forme donnée en paramètre
	// à gauche de la liste lst (en tête)

	if ( animation )
		ajouter_a_gauche_animation((*lst), couleur, forme);

	Token *token = allouer_token(couleur, forme);
	if (token == NULL) {
		return 0;
	}

	if ((*lst) == NULL) {
		(*lst) = token;
	}

	Liste debut = (*lst);

	// Le suivant du token est le premier actuel
	(*token).suivant = (*lst);

	// On va au bout de la liste
	for (; (*(*lst)).suivant != debut; (*lst) = (*(*lst)).suivant );

	// Le suivant du dernier est le token
	(*(*lst)).suivant = token;

	actualise_chainage(*lst);

	// La liste débute maintenant à partir de ce token
	(*lst) = token;

	return 1;

}

int ajouter_a_droite(Liste *lst, int couleur, int forme, int animation) {
	// Ajoute un token de couleur et forme donnée en paramètre
	// à droite de la liste lst (à la fin)

	// Cette fonction est similaire à ajouter_a_gauche()
	// sauf que le début de la liste ne change pas

	if ( animation )
		ajouter_a_droite_animation((*lst), couleur, forme);

	Token *token = allouer_token(couleur, forme);
	if (token == NULL) {
		return 0;
	}

	if ((*lst) == NULL) {
		(*lst) = token;
	}

	Liste debut = (*lst);

	(*token).suivant = (*lst);

	for (; (*(*lst)).suivant != debut; (*lst) = (*(*lst)).suivant );

	(*(*lst)).suivant = token;

	actualise_chainage(*lst);

	(*lst) = debut;
	
	return 1;
}

Token* extrait_token(Liste *lst, Token *token) {
	// Extrait de la liste un token dont on connait l'adresse

	if ( len(*lst) == 0 ) {
		return NULL;
	}

	if ( len(*lst) == 1 ) {
		Liste tmp = *lst;
		(*lst) = NULL;
		return tmp;
	}

	Liste debut;

	if ( (*lst) == token ) {
		// On se place derrière le token à extraire
		debut = (*(*lst)).suivant;
		for (; (*(*lst)).suivant != token; (*lst) = (*(*lst)).suivant);
	} else {
		debut = (*lst);
		if ( (*(*lst)).suivant != token ) {
			(*lst) = (*(*lst)).suivant;

			for (; (*(*lst)).suivant != token && (*lst) != debut ; (*lst) = (*(*lst)).suivant);

			if ( (*lst) == debut )
				// Le token à extraire n'est pas dans la liste
				return NULL;
		}
	}

	// A partir d'ici, on se trouve avant le token à extraire

	Token *tmp = (*(*lst)).suivant;
	(*(*lst)).suivant = (*token).suivant;

	(*lst) = debut;

	actualise_chainage(*lst);

	return tmp;
}

int detecte_couleurs_consecutives(Liste lst, int couleur, Token **token1, Token **token2, Token **token3) {
	// Renvois 1 si un triplet de token consécutif a la même couleur,
	// et 0 dans le cas contraire. token1, token2 et token3 sont l'adresse du triplet
	// de ces tokens, token1 étant le token le plus à gauche.

	if ( len_couleur(lst, couleur) < 3 ) {
		return 0;
	}

	// On récupère le premier tokken ayant la couleur donnée en paramètre
	if ( (*lst).couleur != couleur ) {
		Liste debut = lst;
		lst = (*lst).suivant;

		for (; (*lst).couleur != couleur && lst != debut; lst = (*lst).suivant);

		if ( lst == debut) {
			return 0;
		}
	}

	Token *token = lst;
	Token *premier_token = token;
	int compteur = 1;

	do {
		if ( (*token).suivant_couleur == (*token).suivant )
			compteur++;
		else
			compteur = 1;
		token = (*token).suivant_couleur;
	} while ( token != premier_token && compteur < 3 );

	if (compteur == 3) {
		*token3 = token;
		*token2 = (*token).precedent_couleur;
		*token1 = (*(*token).precedent_couleur).precedent_couleur;
		return 1;
	} else if ( compteur == 2 && (*token).suivant_couleur == (*token).suivant ) {
		*token3 = (*token).suivant_couleur;
		*token2 = token;
		*token1 = (*token).precedent_couleur;
		return 1;
	} else {
		return 0;
	}
}

int detecte_formes_consecutives(Liste lst, int forme, Token **token1, Token **token2, Token **token3) {
	// Renvois 1 si un triplet de token consécutif a la même forme,
	// et 0 dans le cas contraire. token1, token2 et token3 sont l'adresse du triplet
	// de ces tokens, token1 étant le token le plus à gauche.

	if ( len_forme(lst, forme) < 3 ) {
		return 0;
	}

	// On récupère le premier tokken ayant la forme donnée en paramètre
	if ( (*lst).forme != forme ) {
		Liste debut = lst;
		lst = (*lst).suivant;

		for (; (*lst).forme != forme && lst != debut; lst = (*lst).suivant);

		if ( lst == debut) {
			return 0;
		}
	}

	Token *token = lst;
	Token *premier_token = token;
	int compteur = 1;

	do {
		if ( (*token).suivant_forme == (*token).suivant )
			compteur++;
		else
			compteur = 1;
		token = (*token).suivant_forme;
	} while ( token != premier_token && compteur < 3 );

	if (compteur == 3) {
		*token3 = token;
		*token2 = (*token).precedent_forme;
		*token1 = (*(*token).precedent_forme).precedent_forme;
		return 2;
	} else if ( compteur == 2 && (*token).suivant_forme == (*token).suivant ) {
		*token3 = (*token).suivant_forme;
		*token2 = token;
		*token1 = (*token).precedent_forme;
		return 2;
	} else {
		return 0;
	}
}

int max(int a,int b){
	if(a>=b)
		return a;
	else
		return b;
}

int verif_max_consecutives(Liste lst,int type, Token **token1, Token **token3, Token **token4, Token **token5){
	//Renvoie 4 si quadruplet 5 si quintuplet ou 3 si triplet
	//type représente couleur (1) ou forme (2)
	if (type==1){
		if ( (*token3)->couleur == (*token3)->suivant->couleur){
			*token4 = (*token3)->suivant;
			if ((*token4)->couleur == (*token4)->suivant->couleur){
				*token5 = (*token4)->suivant;
				return 5;
			}else if((*token1)->precedent_couleur->suivant==*token1){
				*token5 = (*token1)->precedent_couleur;
				return 5;
			}
			return 4;
		}else if( (*token1)->precedent_couleur->suivant == *token1){
				*token4 = (*token1)->precedent_couleur;
				return 4;
		}else{
			return 3;
		}
	}else{
		if ( (*token3)->forme == (*token3)->suivant->forme){
			*token4 = (*token3)->suivant;
			if ( (*token4)->forme == (*token4)->suivant->forme){
				*token5 = (*token4)->suivant;
				return 5;
			}else if( (*token1)->precedent_forme->suivant == *token1){
				*token5 = (*token1)->precedent_forme;
				return 5;
			}
			return 4;
		}else if( (*token1)->precedent_forme->suivant == *token1){
				*token4 = (*token1)->precedent_forme;
				return 4;
		}else{
			return 3;
		}
	}
	return 0;
}



void libere_liste(Liste *lst) {
	// Libère tous les tokens de la liste et met lst à NULL

	if ( len(*lst) == 0 )
		return;

	Liste debut = *lst;
	Token *tmp;

	do {
		tmp = *lst;
		*lst = (*(*lst)).suivant;
		free(tmp);
	} while ( *lst != debut );

	*lst = NULL;
}

int decalage_circulaire_couleur(Liste lst, int couleur) {
	Liste debut = lst;

	Token* token = NULL;

	// On cherche le premier token ayant la couleur donnée en paramètre
	do {
		if ( (*lst).couleur == couleur )
			token = lst;
		lst = (*lst).suivant;
	} while ( lst != debut && token == NULL );

	if (token == NULL) {
		// Aucun token ayant la couleur demandée existe dans la liste
		return 0;
	}

	Token* premier = token;
	int premiere_forme = (*premier).forme;

	// On effectue le décalage circulaire des couleurs
	do {
		(*token).forme = (*(*token).suivant_couleur).forme;
		token = (*token).suivant_couleur;
	} while (token != premier);

	(*(*token).precedent_couleur).forme = premiere_forme;

	actualise_chainage(lst);

	return 1;
}

int decalage_circulaire_forme(Liste lst, int forme) {
	Liste debut = lst;

	Token* token = NULL;

	// On cherche le premier token ayant la forme donnée en paramètre
	do {
		if ( (*lst).forme == forme )
			token = lst;
		lst = (*lst).suivant;
	} while ( lst != debut && token == NULL );

	if (token == NULL)
		// Aucun token ayant la forme demandée existe dans la liste
		return 0;

	Token* premier = token;
	int premiere_couleur = (*premier).couleur;

	// On effectue le décalage circulaire des formes
	do {
		(*token).couleur = (*(*token).suivant_forme).couleur;
		token = (*token).suivant_forme;
	} while (token != premier);

	(*(*token).precedent_forme).couleur = premiere_couleur;

	actualise_chainage(lst);

	return 1;
}

void sauvegarder(Liste lst, int temps_ecoule, int score, int couleur, int forme){
	/*crée un fichier texte avec les tokens dans la liste en premiere ligne
	et le suivant en 2eme ligne*/

	FILE* fichier= NULL;

	Liste tmp=lst;

	fichier=fopen("sauvegarde.txt","w+");

	fprintf(fichier,"%d\n", len(lst));

	fprintf(fichier,"%d\n", temps_ecoule);

	fprintf(fichier,"%d\n", score);

	fprintf(fichier,"%d %d\n", couleur, forme);

	if ( lst != NULL ) {
		do {
			fprintf(fichier, "%d %d,", (*lst).couleur, (*lst).forme);
			lst = (*lst).suivant;
		} while (lst != tmp);

		fprintf(fichier,"\n");
	}

	fclose(fichier);
}

int charger(Liste* lst, int *temps_ecoule, int *score, int *couleur_prochain, int *forme_prochain){
	/*renvoie par adresse la couleur et la forme du prochain token
	ainsi que la liste modifiée selon le fichier*/

	int taille, couleur, forme, i = 0;

	FILE* fichier= NULL;

	fichier=fopen("sauvegarde.txt","r");

	if (fichier == NULL)
		return 0;

	if ( !fscanf(fichier, "%d\n%d\n%d\n%d %d", &taille, temps_ecoule, score, couleur_prochain, forme_prochain) )
		printf("1 - Echec de la lecture du fichier sauvegarde\n");

	if ( taille != 0 ) {
		while ( i < taille) {
			if ( !fscanf(fichier, "%d %d,", &couleur, &forme) )
				printf("2 - Echec de la lecture du fichier sauvegarde\n");
			ajouter_a_droite(lst, couleur, forme, 0);
			i++;
		}
	}

	fclose(fichier);

	return 1;
}