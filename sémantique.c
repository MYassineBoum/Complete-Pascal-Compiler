#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    ID_TOKEN,
    PROGRAM_TOKEN,
    CONST_TOKEN,
    VAR_TOKEN,
    BEGIN_TOKEN,
    END_TOKEN,
    IF_TOKEN,
    THEN_TOKEN,
    WHILE_TOKEN,
    DO_TOKEN,
    READ_TOKEN,
    WRITE_TOKEN,
    PV_TOKEN,
    PT_TOKEN,
    PLUS_TOKEN,
    MOINS_TOKEN,
    MULT_TOKEN,
    DIV_TOKEN,
    VIR_TOKEN,
    AFF_TOKEN,
    INF_TOKEN,
    INFEG_TOKEN,
    SUP_TOKEN,
    SUPEG_TOKEN,
    DIFF_TOKEN,
    PO_TOKEN,
    PF_TOKEN,
    FIN_TOKEN,
    NUM_TOKEN,
    ERREUR_TOKEN,
    EOF_TOKEN,
    EG_TOKEN,REPEAT_TOKEN,UNTIL_TOKEN,FOR_TOKEN,ELSE_TOKEN,CASE_TOKEN,OF_TOKEN,
    INTO_TOKEN,DOWNTO_TOKEN,
    DDOT_TOKEN
} CODES_LEX;

// erreur types
typedef enum
{
    ID_ERR,
    PROGRAM_ERR,
    CONST_ERR,
    VAR_ERR,
    BEGIN_ERR,
    END_ERR,
    IF_ERR,
    THEN_ERR,
    WHILE_ERR,
    DO_ERR,
    READ_ERR,
    WRITE_ERR,
    PV_ERR,
    PT_ERR,
    PLUS_ERR,
    MOINS_ERR,
    MULT_ERR,
    DIV_ERR,
    VIR_ERR,
    AFF_ERR,
    INF_ERR,
    INFEG_ERR,
    SUP_ERR,
    SUPEG_ERR,
    DIFF_ERR,
    PO_ERR,
    PF_ERR,
    FIN_ERR,
    NUM_ERR,
    ERREUR_ERR,
    EOF_ERR,
    EG_ERR,
    CONST_VAR_BEGIN_ERR,
    VAR_BEGIN_ERR,REPEAT_ERR,UNTIL_ERR,FOR_ERR,ELSE_ERR,CASE_ERR,OF_ERR,
    INTO_ERR,DOWNTO_ERR,DDOT_ERR
} CODES_ERR;

typedef struct
{
    CODES_LEX CODE;
    char NOM[20];
} TSym_Cour;

TSym_Cour SYM_COUR;

FILE * fichier;

char Car_Cour; // caractère courant

void lire_mot()
{
    char mot[20];
    int indice = 0;

    // Lecture du premier caractère (lettre)
    mot[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des caractères suivants (lettres ou chiffres)
    while (isalpha(Car_Cour) || isdigit(Car_Cour))
    {
        mot[indice++] = Car_Cour;
        Lire_Car();
    }

    // Ajout du caractère de fin de chaîne
    mot[indice] = '\0';

    // Vérifier si le mot est un mot-clé
    if (stricmp(mot, "program") == 0)
    {
        SYM_COUR.CODE = PROGRAM_TOKEN;
    }
    else if (stricmp(mot, "const") == 0)
    {
        SYM_COUR.CODE = CONST_TOKEN;
    }
    else if (stricmp(mot, "var") == 0)
    {
        SYM_COUR.CODE = VAR_TOKEN;
    }
    else if (stricmp(mot, "begin") == 0)
    {
        SYM_COUR.CODE = BEGIN_TOKEN;
    }
    else if (stricmp(mot, "end") == 0)
    {
        SYM_COUR.CODE = END_TOKEN;
    }
    else if (stricmp(mot, "if") == 0)
    {
        SYM_COUR.CODE = IF_TOKEN;
    }
    else if (stricmp(mot, "then") == 0)
    {
        SYM_COUR.CODE = THEN_TOKEN;
    }
    else if (stricmp(mot, "while") == 0)
    {
        SYM_COUR.CODE = WHILE_TOKEN;
    }
    else if (stricmp(mot, "do") == 0)
    {
        SYM_COUR.CODE = DO_TOKEN;
    }
    else if (stricmp(mot, "read") == 0)
    {
        SYM_COUR.CODE = READ_TOKEN;
    }
    else if (stricmp(mot, "write") == 0)
    {
        SYM_COUR.CODE = WRITE_TOKEN;
    }
    else if (stricmp(mot, "else") == 0)
    {
        SYM_COUR.CODE = ELSE_TOKEN;
    }
    else if (stricmp(mot, "repeat") == 0)
    {
        SYM_COUR.CODE = REPEAT_TOKEN;
    }
    else if (stricmp(mot, "until") == 0)
    {
        SYM_COUR.CODE = UNTIL_TOKEN;
    }
    else if (stricmp(mot, "for") == 0)
    {
        SYM_COUR.CODE = FOR_TOKEN;
    }
    else if (stricmp(mot, "case") == 0)
    {
        SYM_COUR.CODE = CASE_TOKEN;
    }
    else if (stricmp(mot, "of") == 0)
    {
        SYM_COUR.CODE = OF_TOKEN;
    }
    else
    {
        // Si ce n'est pas un mot-clé, c'est un identifiant
        SYM_COUR.CODE = ID_TOKEN;
    }

    // Stockage du mot dans le jeton
    strcpy(SYM_COUR.NOM, mot);
}

void lire_nombre()
{
    char nombre[11];
    int indice = 0;

    // Lecture du premier chiffre
    nombre[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des chiffres suivants
    while (isdigit(Car_Cour))
    {
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }

    // Ajout du caractère de fin de chaîne
    nombre[indice] = '\0';

    // Stockage du nombre dans le jeton
    SYM_COUR.CODE = NUM_TOKEN;
    strcpy(SYM_COUR.NOM, nombre);
}

void Sym_Suiv()
{
    while (Car_Cour == ' ' || Car_Cour == '\n' || Car_Cour == '\t')
    {
        Lire_Car();
    }
    if (isalpha(Car_Cour))
    {
        lire_mot();
    }
    else if (isdigit(Car_Cour))
    {
        lire_nombre();
    }
    else
    {
        switch (Car_Cour)
        {
        case ';':
            SYM_COUR.CODE = PV_TOKEN;
            Lire_Car();
            break;

        case '+':
            SYM_COUR.CODE = PLUS_TOKEN;
            Lire_Car();
            break;

        case '-':
            SYM_COUR.CODE = MOINS_TOKEN;
            Lire_Car();
            break;

        case '*':
            SYM_COUR.CODE = MULT_TOKEN;
            Lire_Car();
            break;

        case '/':
            SYM_COUR.CODE = DIV_TOKEN;
            Lire_Car();
            break;

        case ',':
            SYM_COUR.CODE = VIR_TOKEN;
            Lire_Car();
            break;

        case ':':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = AFF_TOKEN;
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = DDOT_TOKEN;
            }
            break;

        case '<':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = INFEG_TOKEN;
                Lire_Car();
            }
            else if (Car_Cour == '>')
            {
                SYM_COUR.CODE = DIFF_TOKEN;
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = INF_TOKEN;
            }
            break;

        case '>':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = SUPEG_TOKEN;
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = SUP_TOKEN;
            }
            break;

        case '(':
            SYM_COUR.CODE = PO_TOKEN;
            Lire_Car();
            break;
        case '=':
            SYM_COUR.CODE = EG_TOKEN;
            Lire_Car();
            break;

        case ')':
            SYM_COUR.CODE = PF_TOKEN;
            Lire_Car();
            break;

        case '.':
            SYM_COUR.CODE = PT_TOKEN;
            Lire_Car();
            break;

        case EOF:
            SYM_COUR.CODE = FIN_TOKEN;
            break;

        default:
            SYM_COUR.CODE = ERREUR_TOKEN;
            Lire_Car();
        }
        strcpy(SYM_COUR.NOM , Car_Cour);
    }

    printf("Symbol: %s\n", SYM_COUR.NOM);
}

void Lire_Car()
{
    Car_Cour = fgetc(fichier);
}

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

T_TAB_IDF TAB_IDFS;

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
			strcpy(TAB_IDFS.NOM, SYM_COUR.NOM);
			TAB_IDFS.TIDF = TPROG;
		}
		Sym_Suiv();
	}
	table_id();
}

void store_const() {
	while((SYM_COUR.CODE != PROGRAM_TOKEN) && (SYM_COUR.CODE != VAR_TOKEN) && (SYM_COUR.CODE != BEGIN_TOKEN)) {
		if(SYM_COUR.CODE == ID_TOKEN) {
			strcpy(TAB_IDFS.NOM, SYM_COUR.NOM);

			TAB_IDFS.TIDF = TCONST;
			Sym_Suiv();
			if(SYM_COUR.CODE == EG_TOKEN) {
				Sym_Suiv();
			    //TAB_IDFS.value = SYM_COUR.value;
		    }
		}
		Sym_Suiv();
	}
	table_id();
}

void store_var() {
	while((SYM_COUR.CODE != CONST_TOKEN) && (SYM_COUR.CODE != PROGRAM_TOKEN) && (SYM_COUR.CODE != BEGIN_TOKEN)) {
		if(SYM_COUR.CODE == ID_TOKEN) {
            regle2();
			strcpy(TAB_IDFS.NOM, SYM_COUR.NOM);
			TAB_IDFS.TIDF = TVAR;
		}
		Sym_Suiv();
	}
	table_id();
}

void affich() {
	regle1();
	regle3();
	regle4();
	regle5();

	if(Y == 0) {
		printf("La sémantique est correcte!\n");
	}
    else printf("La sementique est erronée!\n");	
}

void regle1() {
	//head=head_sem ;
	while(SYM_COUR.CODE != BEGIN_TOKEN) {
		if(SYM_COUR.CODE == ID_TOKEN) {
			check();
		}
		Sym_Suiv();
	}
}

void check() {
	int r = 0; 
	for (int i = 0; i < sizeof(TAB_IDFS); ++i) {
		if(strcmp(SYM_COUR.NOM, TAB_IDFS.NOM) == 0) {
			r = 1;
		}
	}

	if(r == 0) {
		printf("%s ----> Erreur declaration hors CONST et VAR!\n", SYM_COUR.NOM);
		Y = 1;
	}
}



void regle2() {
	int r = 0;
	for (int i = 0; i < sizeof(TAB_IDFS); ++i) {
		if(strcmp(SYM_COUR.NOM,TAB_IDFS.NOM) == 0) {
			r = 1;
		}
	}

	if(r == 1) {
		printf("%s ----> Erreur variable deja declaree!\n ", SYM_COUR.NOM);
		Y = 1;
	}
}

void regle3() {
	//head=head_sem ;
	while(SYM_COUR.CODE != BEGIN_TOKEN) Sym_Suiv();

	while(SYM_COUR.CODE != PT_TOKEN) {
		if(SYM_COUR.CODE == ID_TOKEN) {    
            int r = 0; 
            for (int i = 0; i < sizeof(TAB_IDFS); ++i) {
                if(strcmp(SYM_COUR.NOM, TAB_IDFS.NOM) == 0) {
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
    //head=head_sem ;
    while(SYM_COUR.CODE != BEGIN_TOKEN) Sym_Suiv();
    while(SYM_COUR.CODE != PT_TOKEN) {
        if(SYM_COUR.CODE ==ID_TOKEN) {
            for (int i = 0; i < sizeof(TAB_IDFS); ++i) {
                if(TAB_IDFS.TIDF == 1) {
                    Sym_Suiv();
                    if(SYM_COUR.CODE ==EG_TOKEN) {
                        Sym_Suiv();
                        /*if(head->data->val!=TAB_IDFS.value) {
                            printf("%s------>Erreur : Une constante ne peut changer de valeur dans le programme\n",SYM_COUR.NOM);
                            Y=1;
                        }*/
                    }     
                }
            }
        }

        if(SYM_COUR.CODE == READ_TOKEN) {
            Sym_Suiv();
            Sym_Suiv();
            if(SYM_COUR.CODE == ID_TOKEN) {
                for (int i = 0; i < sizeof(TAB_IDFS) ; ++i) {
                    if(strcmp(SYM_COUR.NOM,TAB_IDFS.NOM)==0) {
                        if(TAB_IDFS.TIDF==1) {
                            printf("%s ----------> constante	ne	peut changer	de	valeur a partire de READ\n",SYM_COUR.NOM);
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
	//head=head_sem ;
	while(SYM_COUR.CODE !=BEGIN_TOKEN) Sym_Suiv();
	while(SYM_COUR.CODE !=PT_TOKEN) {
        if(SYM_COUR.CODE ==ID_TOKEN) {
            for (int i = 0; i <sizeof(TAB_IDFS) ; ++i) {
                if(strcmp(SYM_COUR.NOM,TAB_IDFS.NOM)==0) {
                    if(TAB_IDFS.TIDF==0) {
                        printf("%s ----------> nom	de	programme	non	autorisé\n",SYM_COUR.NOM);
                        Y=1;
                    }
                }
            }
        }
        Sym_Suiv();
	}
}