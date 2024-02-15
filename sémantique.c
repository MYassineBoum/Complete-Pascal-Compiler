#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntaxique.c"

typedef enum {
    TPROG,
    TCONST,
    TVAR
} TSYM;

typedef struct {
    TSYM TIDF;
    char NOM[20];
    int value;
} T_TAB_IDF;

T_TAB_IDF * TAB_IDFS;


void table_id();
void store_prog();
void store_const();
void store_var();
void affich();
void regle1();
void check();
void regle2();
void regle3();
void regle4();
void regle5();

int i = 0;
int Y = 0;

void table_id() {
    switch(SYM_COUR.CODE) {
        case PROGRAM_TOKEN : Sym_Suiv(); store_prog(); break;
        case CONST_TOKEN : Sym_Suiv(); store_const(); break;
        case VAR_TOKEN : Sym_Suiv(); store_var(); break;
        default : ; break;
    }
}

void store_prog() {
	while((SYM_COUR.CODE != CONST_TOKEN) && (SYM_COUR.CODE != VAR_TOKEN) && (SYM_COUR.CODE != BEGIN_TOKEN)) {
		if(SYM_COUR.CODE == ID_TOKEN) {
			strcpy((TAB_IDFS+i)->NOM, SYM_COUR.NOM);
			(TAB_IDFS+i)->TIDF = TPROG;
            i++;
		}
		Sym_Suiv();
	}
    printf("tprog %s \n", SYM_COUR.NOM);
	table_id();
}

void store_const() {
	while((SYM_COUR.CODE != PROGRAM_TOKEN) && (SYM_COUR.CODE != VAR_TOKEN) && (SYM_COUR.CODE != BEGIN_TOKEN)) {
		if(SYM_COUR.CODE == ID_TOKEN) {
			strcpy((TAB_IDFS+i)->NOM, SYM_COUR.NOM);
			(TAB_IDFS+i)->TIDF = TCONST;
			Sym_Suiv();
			if(SYM_COUR.CODE == EG_TOKEN) {
				Sym_Suiv();
			    (TAB_IDFS+i)->value = SYM_COUR.val;
		    }
            i++;
		}
		Sym_Suiv();
		Sym_Suiv();
		printf("tconst %s \n", SYM_COUR.NOM);
	}
	table_id();
}

void store_var() {
	while((SYM_COUR.CODE != CONST_TOKEN) && (SYM_COUR.CODE != PROGRAM_TOKEN) && (SYM_COUR.CODE != BEGIN_TOKEN)) {
		if(SYM_COUR.CODE == ID_TOKEN) {
            regle2();
			strcpy((TAB_IDFS+i)->NOM, SYM_COUR.NOM);
			(TAB_IDFS+i)->TIDF = TVAR;
            i++;
		}
		Sym_Suiv();
	}
    printf("tvar %s \n", SYM_COUR.NOM);
	table_id();
}

void affich() {
	Lire_Car();
    Sym_Suiv();
	regle1();
	printf("regle1\n");
	regle3();
	printf("regle3\n");
	regle4();
	printf("regle4\n");
	regle5();

	if(Y == 0) {
		printf("La semantique est correcte!\n");
	}
    else printf("La semantique est erronee!\n");	
}

void regle1() {
    printf("%s en regle1\n", SYM_COUR.NOM);
	while(SYM_COUR.CODE != EOF_TOKEN) {
		if(SYM_COUR.CODE == ID_TOKEN) {
			printf("%s before check\n", SYM_COUR.NOM);
			check();
		}
		Sym_Suiv();
	}
}

void check() {
	int r = 0; 
	int i;
	for (i = 0; i < 4; ++i) {
		if(strcmp(SYM_COUR.NOM, (TAB_IDFS+i)->NOM) == 0) {
			r = 1;
		}
	}

	if(r == 0) {
		//printf("%s TAB IDF\n", (TAB_IDFS+1)->NOM);
		printf("%s ----> Erreur declaration hors CONST et VAR 1!\n", SYM_COUR.NOM);
		Y = 1;
	} else {
		printf("%s GOOD\n", SYM_COUR.NOM);
		/*printf("%s TAB IDF\n", (TAB_IDFS+0)->NOM);
		printf("%s TAB IDF\n", (TAB_IDFS+1)->NOM);
		printf("%s TAB IDF\n", (TAB_IDFS+2)->NOM);
		printf("%s TAB IDF\n", (TAB_IDFS+3)->NOM);
		printf("%s TAB IDF\n", (TAB_IDFS+4)->NOM);*/
	}
	
}

void regle2() {
	int r = 0;
	int i;
	for (i = 0; i < 4; ++i) {
		if(strcmp(SYM_COUR.NOM, (TAB_IDFS+i)->NOM) == 0) {
			r = 1;
		}
	}

	if(r == 1) {
		printf("%s ----> Erreur variable deja declaree!\n ", SYM_COUR.NOM);
		Y = 1;
	} else {
		printf("%s continue\n", SYM_COUR.NOM);
	}
}

void regle3() {
	while(SYM_COUR.CODE != BEGIN_TOKEN) Sym_Suiv();

	while(SYM_COUR.CODE != PT_TOKEN) {
		if(SYM_COUR.CODE == ID_TOKEN) {    
            int r = 0; 
            int i;
            for (i = 0; i < 4; ++i) {
                if(strcmp(SYM_COUR.NOM, (TAB_IDFS+i)->NOM) == 0) {
                    r = 1;
                }
            }

            if(r == 0) {
                printf("%s ----> Erreur variable deja declaree!\n", SYM_COUR.NOM);
                Y = 1;
            }
        }
        Sym_Suiv();
	}
}

void regle4() {

    while(SYM_COUR.CODE != BEGIN_TOKEN) Sym_Suiv();
    while(SYM_COUR.CODE != PT_TOKEN) {
        if(SYM_COUR.CODE == ID_TOKEN) {
        	int i;
            for (i = 0; i < 4; ++i) {
                if((TAB_IDFS+i)->TIDF == 1) {
                    Sym_Suiv();
                    if(SYM_COUR.CODE == EG_TOKEN) {
                        Sym_Suiv();
                        if(SYM_COUR.val != (TAB_IDFS+i)->value) {
                            printf("%s ----> Erreur, une constante ne peut pas changer de valeur dans le programme!\n", SYM_COUR.NOM);
                            Y = 1;
                        }
                    }     
                }
            }
        }

        if(SYM_COUR.CODE == READ_TOKEN) {
            Sym_Suiv();
            Sym_Suiv();
            if(SYM_COUR.CODE == ID_TOKEN) {
            	int i;
                for (i = 0; i < 4; ++i) {
                    if(strcmp(SYM_COUR.NOM, (TAB_IDFS+i)->NOM) == 0) {
                        if((TAB_IDFS+i)->TIDF == 1) {
                            printf("%s ----> Erreur, une constante ne peut pas changer de valeur a partir de READ!\n", SYM_COUR.NOM);
                            Y = 1;
                        }	
                    }
                }
            }
        }
        Sym_Suiv();
    }
}

void regle5() {

	while(SYM_COUR.CODE != BEGIN_TOKEN) Sym_Suiv();
	while(SYM_COUR.CODE != PT_TOKEN) {
        if(SYM_COUR.CODE == ID_TOKEN) {
        	int i;
            for (i = 0; i < 4; ++i) {
                if(strcmp(SYM_COUR.NOM, (TAB_IDFS+i)->NOM) == 0) {
                    if((TAB_IDFS+i)->TIDF == 0) {
                        printf("%s ----> Nom de	programme non autorise!\n", SYM_COUR.NOM);
                        Y = 1;
                    }
                }
            }
        }
        Sym_Suiv();
	}
}

int main() {
    fichier = fopen("program.p", "r");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier!");
        return 1;
    }

    // PREMIER_SYM();
    Lire_Car();
    Sym_Suiv();
    TAB_IDFS = (T_TAB_IDF *)malloc(sizeof(T_TAB_IDF) * 20);
    //TAB_IDFS = malloc(sizeof(T_TAB_IDF) * NbrIDFS);
    table_id();
    affich();
    
    fclose(fichier);

    
    printf("Execution du programme faite.\n");
/*
    if (SYM_COUR.CODE == EOF_TOKEN)
    {
        printf("Le programme est correcte!\n");
    }
    else
    {
        printf("Votre programme est erroné!\n");
        printf("Current Token: %d\n", SYM_COUR.CODE);
        printf("Current Lexeme: %s\n", SYM_COUR.NOM);
        Sym_Suiv(); // Move this line inside the else block
    }
    */
    fclose(fichier);

    return 0;
}
