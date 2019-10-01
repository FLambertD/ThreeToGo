typedef struct token {
	int couleur;
	int forme;
	struct token* suivant;
	struct token* suivant_couleur;
	struct token* precedent_couleur;
	struct token* suivant_forme;
	struct token* precedent_forme;
} Token, *Liste;

typedef struct zone {
// Structure pour définir la surface d'une zone rectangulaire.
// Utilisable surtout pour les boutons.
	int x1;
	int y1;
	int x2;
	int y2;
} Zone;