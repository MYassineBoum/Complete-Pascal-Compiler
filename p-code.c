#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLEINDEX 100
#define TAILLEMEM 100
#define TAILLECODE 100

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
    EG_TOKEN,
    REPEAT_TOKEN,
    UNTIL_TOKEN,
    FOR_TOKEN,
    ELSE_TOKEN,
    CASE_TOKEN,
    OF_TOKEN,
    INTO_TOKEN,
    DOWNTO_TOKEN,
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
    VAR_BEGIN_ERR,
    REPEAT_ERR,
    UNTIL_ERR,
    FOR_ERR,
    ELSE_ERR,
    CASE_ERR,
    OF_ERR,
    INTO_ERR,
    DOWNTO_ERR,
    DDOT_ERR
} CODES_ERR;

int isReadActivated = 0;

typedef struct
{
    CODES_LEX CODE;
    char NOM[20];
    int val;
} TSym_Cour;

TSym_Cour head;
TSym_Cour SYM_COUR;

FILE *fichier;

char Car_Cour; // caractère courant

typedef enum
{
    TPROG,
    TCONST,
    TVAR
} TSYM;

typedef struct
{
    TSYM TIDF;
    char NOM[20];
    int value;
} T_TAB_IDF;

T_TAB_IDF *TAB_IDFS;

int k = 0;

typedef enum
{
    RPROG,
    RCONST,
    RVAR,
    RBEGIN
} Region;

Region current_region = TPROG;

TSYM lastType = TPROG;
char lastIdToken[20];

typedef struct
{
	char NOM[20];
	CODES_LEX CLASSE;
	int ADRESSE;
} TSYMB;

TSYMB TABLESYM[TABLEINDEX];
int IND_DER_SYM_ACC = 0; // Index dans TABLESYM
int OFFSET = -1;

int MEM[TAILLEMEM]; // Mémoire (Pile de la machine)
int SP; // Pointeur vers le sommet du pile

typedef enum
{
	ADD,SUB,MUL,DIV,EQL,NEQ,GTR,
	LSS,GEQ,LEQ, PRN,INN,INT,LDI,LDA,LDV,
	STO,BRN,BZE,HLT
} MNEMONIQUES;

typedef struct
{
	MNEMONIQUES MNE; // Instru
	int SUITE; // Son suivant
} INSTRUCTION;

INSTRUCTION PCODE[TAILLECODE];

int LABEL_BRN;
int INDICE_BZE;
int IND_BZE;

int PC = 0; // Compteur d'instructions

int operation = 0;

// Prototypes des fonctions à utiliser
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANTQUE();
void ECRIRE();
void LIRE();
void EXPR();
void TERM();
void FACT();
void MULOP();
void ADDOP();
void RELOP();
void COND();
void Lire_Car();
void Erreur(CODES_ERR code);
void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR);
void PROGRAM();
void BLOCK();
void CONSTS();
void Sym_Suiv();
void lire_mot();
void lire_nombre();
void CAS();
void POUR();
void REPETER();

void Check();

//P CODE section
void GENERER1(MNEMONIQUES M);
void GENERER2(MNEMONIQUES M, int A);

// Definition des fonctions à utiliser

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
        isReadActivated = 1;
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
    else if (stricmp(mot, "to") == 0)
    {
        SYM_COUR.CODE = INTO_TOKEN;
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
        // Repérer les ID_TOKEN par des adresses
        //TABLESYM[IND_DER_SYM_ACC].NOM = SYM_COUR.NOM;
        //TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
        //TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
        //IND_DER_SYM_ACC++;
        // Stockage du mot dans le jeton
        strcpy(SYM_COUR.NOM, mot);
        Check();
    }
    if (SYM_COUR.CODE != ID_TOKEN)
    {
        // Stockage du mot dans le jeton
        strcpy(SYM_COUR.NOM, mot);
    }
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
    SYM_COUR.val = atoi(SYM_COUR.NOM);
}

int main()
{
    fichier = fopen("program.p", "r");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier!");
        return 1;
    }

    // Initialize TAB_IDFS
    TAB_IDFS = malloc(100 * sizeof(T_TAB_IDF));
    if (TAB_IDFS == NULL)
    {
        perror("Erreur lors de l'allocation de mémoire pour TAB_IDFS!");
        return 1;
    }

    Lire_Car();
    Sym_Suiv();

    PROGRAM();

    printf("Execution du programme faite.\n");

    fclose(fichier);
    free(TAB_IDFS); // Free allocated memory

    return 0;
}


void Check()
{
    int r = 0;
    int i;
    switch (current_region)
    {
    case RPROG:
        for (i = 0; i < k; ++i)
        {
            if (strcmp(SYM_COUR.NOM, TAB_IDFS[i].NOM) == 0)
            {
                r = 1;
                break;
            }
        }

        if (r == 1)
        {
            if (TPROG == TAB_IDFS[i].TIDF)
            {
                printf("%s ----> Erreur:  Le ID du programme ne peut etre utilise dans le programme.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("%s ----> Erreur: PAS DE DOUBLE DECLARATIONS.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
        }
        strcpy(TAB_IDFS[k].NOM, SYM_COUR.NOM);
        TAB_IDFS[k].TIDF = TPROG;
        k++;

        break;
    case RCONST:
        for (i = 0; i < k; ++i)
        {
            if (strcmp(SYM_COUR.NOM, TAB_IDFS[i].NOM) == 0)
            {
                r = 1;
                break;
            }
        }

        if (r == 1)
        {
            if (TPROG == TAB_IDFS[i].TIDF)
            {
                printf("%s ----> Erreur:  Le ID du programme ne peut etre utilise dans le programme.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("%s ----> Erreur: PAS DE DOUBLE DECLARATIONS.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            strcpy(TAB_IDFS[k].NOM, SYM_COUR.NOM);
            TAB_IDFS[k].TIDF = TCONST;
            k++;
        }

        break;
    case RVAR:
        for (i = 0; i < k; ++i)
        {
            if (strcmp(SYM_COUR.NOM, TAB_IDFS[i].NOM) == 0)
            {
                r = 1;
                break;
            }
        }

        if (r == 1)
        {
            if (TPROG == TAB_IDFS[i].TIDF)
            {
                printf("%s ----> Erreur:  Le ID du programme ne peut etre utilise dans le programme.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("%s ----> Erreur: PAS DE DOUBLE DECLARATIONS.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            strcpy(TAB_IDFS[k].NOM, SYM_COUR.NOM);
            TAB_IDFS[k].TIDF = TVAR;
            k++;
        }

        break;
    case RBEGIN:
        for (i = 0; i < k; ++i)
        {
            if (strcmp(SYM_COUR.NOM, TAB_IDFS[i].NOM) == 0)
            {
                r = 1;
                break;
            }
        }

        if (r == 1)
        {
            if (TPROG == TAB_IDFS[i].TIDF)
            {
                printf("%s ----> Erreur:  Le ID du programme ne peut etre utilise dans le programme.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            printf("%s ----> Erreur:  Tous les symboles doivent etre deja declares.", SYM_COUR.NOM);
            exit(EXIT_FAILURE);
        }
        if (isReadActivated)
        {
            isReadActivated = 0;
            if (TCONST == TAB_IDFS[i].TIDF)
            {
                printf("%s ----> Erreur:  Une constante ne peut changer de valeur dans le programme.", SYM_COUR.NOM);
                exit(EXIT_FAILURE);
            }
        }
        break;
    default:
        break;
    }

    lastType = TAB_IDFS[i].TIDF;
    strcpy(lastIdToken, TAB_IDFS[i].NOM);
}

void Lire_Car()
{
    Car_Cour = fgetc(fichier);
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
        strcpy(SYM_COUR.NOM, &Car_Cour);
    }

    // printf("Symbol: %s\n", SYM_COUR.NOM);
}

void Erreur(CODES_ERR code)
{
    printf("Erreur: %d\n", code);
    printf("Current Token: %d\n", SYM_COUR.CODE);
    printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

// Fonctions de génération du P-CODE

void GENERER1(MNEMONIQUES M) {
	if (PC == TAILLECODE) {
		exit(EXIT_FAILURE);
	}
	PC++;
	PCODE[PC].MNE = M;
}

void GENERER2(MNEMONIQUES M, int A) {
	if (PC == TAILLECODE) {
		exit(EXIT_FAILURE);
	}
	PC++;
	PCODE[PC].MNE = M;
	PCODE[PC].SUITE = A;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void SaveInstToFile(INSTRUCTION INST, int i) {
	switch( INST.MNE){
		case LDA: fprintf(FICH_SORTIE, "%s \t %d \n", "LDA", INST.SUITE); break;
		case LDI: fprintf(FICH_SORTIE, "%s \t %d \n", "LDI", INST.SUITE); break;
		case INT: fprintf(FICH_SORTIE, "%s \t %d \n", "INT", INST.SUITE); break;
		case BZE: fprintf(FICH_SORTIE, "%s \t %d \n", "BZE", INST.SUITE); break;
		case BRN: fprintf(FICH_SORTIE, "%s \t %d \n", "BRN", INST.SUITE); break;
		case LDV: fprintf(FICH_SORTIE, "%s \n", "LDV"); break;
		case ADD: fprintf(FICH_SORTIE, "%s \n", "ADD"); break;
		case SUB: fprintf(FICH_SORTIE, "%s \n", "SUB"); break;
		case MUL: fprintf(FICH_SORTIE, "%s \n", "MUL"); break;
		case DIV: fprintf(FICH_SORTIE, "%s \n", "DIV"); break;
		case LEQ: fprintf(FICH_SORTIE, "%s \n", "LEQ"); break;
		case GEQ: fprintf(FICH_SORTIE, "%s \n", "GEQ"); break;
		case NEQ: fprintf(FICH_SORTIE, "%s \n", "NEQ"); break;
		case LSS: fprintf(FICH_SORTIE, "%s \n", "LSS"); break;
		case GTR: fprintf(FICH_SORTIE, "%s \n", "GTR"); break;
		case HLT: fprintf(FICH_SORTIE, "%s \n", "HLT"); break;
		case STO: fprintf(FICH_SORTIE, "%s \n", "STO"); break;
		case INN: fprintf(FICH_SORTIE, "%s \n", "INN"); break;
		case PRN: fprintf(FICH_SORTIE, "%s \n", "PRN"); break;
		default: Erreur(INST_PCODE_ERR); break;
 	}
}
*/
void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR)
{
    if (SYM_COUR.CODE == cl)
    {
        Sym_Suiv();
    }
    else
        Erreur(COD_ERR);
}

void PROGRAM()
{
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    current_region = RPROG;
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    Test_Symbole(PT_TOKEN, PT_ERR);
}

void BLOCK()
{
    current_region = RCONST;
    CONSTS();
    current_region = RVAR;
    VARS();
    current_region = RBEGIN;
    INSTS();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CONSTS()
{
    switch (SYM_COUR.CODE)
    {
    case CONST_TOKEN:
        Sym_Suiv();
        
        // Définir une nouvelle constante en mémoire
        TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
        strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
        TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
        
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
        
        Test_Symbole(ID_TOKEN, ID_ERR);
        Test_Symbole(EG_TOKEN, EG_ERR);
        
        // Empiler la valeur lue (correspond à NUM_TOKEN)
        GENERER2(LDI, SYM_COUR.val);
        GENERER1(STO);
        
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        
        IND_DER_SYM_ACC++;
        
        Test_Symbole(PV_TOKEN, PV_ERR);
        while (SYM_COUR.CODE == ID_TOKEN)
        {
        	// Définir une nouvelle constante en mémoire
        	TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN.NOM;
        	strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
        	TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
        	GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
        	
            Sym_Suiv();
        	
            Test_Symbole(EG_TOKEN, EG_ERR);
            
            // Empiler la valeur lue (correspon à NUM_TOKEN)
            GENERER2(LDI, SYM_COUR.val);
            GENERER1(STO);
            
            Test_Symbole(NUM_TOKEN, NUM_ERR);
            
            IND_DER_SYM_ACC++;
            
            Test_Symbole(PV_TOKEN, PV_ERR);
        };
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR);
        break;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Codage_Lex(char mot[20])
{
	int indice_token=-1;
	indice_token=RechercherSym(mot);
	if (indice_token!=-1) {
		if ((AVANT_BEGIN==1) && (indice_token>10)) ERREUR(DD_ERR);
		else {
			SYM_COUR.CLS=TABLESYM[indice_token].CLS;
			IND_DER_SYM_ACC=indice_token;
		}
	}
	else {
		if (AVANT_BEGIN==1) {
		 	SYM_COUR.CLS=ID_TOKEN;
		 	IND_DER_SYM_ACC=index_Mots;
		 	AJOUTER();
		}
	 	else ERREUR(ND_ERR);
	}
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VARS()
{
    switch (SYM_COUR.CODE)
    {
    case VAR_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);

        while (SYM_COUR.CODE == VIR_TOKEN)
        {
            Sym_Suiv();
            Test_Symbole(ID_TOKEN, ID_ERR);
        }

        Test_Symbole(PV_TOKEN, PV_ERR);
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(VAR_BEGIN_ERR);
        break;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void INSTS()
{
    // begin INST { ; INST } end
    if (SYM_COUR.CODE == BEGIN_TOKEN)
    {
        Sym_Suiv();
        INST();

        while (SYM_COUR.CODE == PV_TOKEN)
        {
            Sym_Suiv();
            INST();
        }

        if (SYM_COUR.CODE == END_TOKEN)
        {
            Sym_Suiv();
            printf("Le programme est correcte!\n");
            // printf("Current Token: %d\n", SYM_COUR.CODE);
            // printf("Current Lexeme: %s\n", SYM_COUR.NOM);
        }
        else
        {
            Erreur(FIN_ERR);
        }
    }
    else
    {
        Erreur(BEGIN_ERR);
    }
}

void INST()

{
    // INSTS | AFFEC | SI | TANTQUE | ECRIRE | LIRE | e
    switch (SYM_COUR.CODE)
    {
    case BEGIN_TOKEN:
        INSTS();
        break;
    case ID_TOKEN:
        AFFEC();
        break;
    case IF_TOKEN:
        SI();
        break;
    case WHILE_TOKEN:
        TANTQUE();
        break;
    case WRITE_TOKEN:
        ECRIRE();
        break;
    case READ_TOKEN:
        LIRE();
        break;
    case REPEAT_TOKEN:
        REPETER();
        break;
    case FOR_TOKEN:
        POUR();
        break;
    case CASE_TOKEN:
        CAS();
        break;
    default:
        break;
    }
}

void AFFEC()
{
	
	// Définir une nouvelle variable en mémoire
    strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
    TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
    TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
    
    // Empiler l'adresse de cette nouvelle variable pour but d'affectation (Voir FACT())
    GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    
    // ID := EXPR
    Test_Symbole(ID_TOKEN, ID_ERR);
    if (TVAR != lastType)
    {
        printf("%s ----> Erreur:  Une constante ne peut changer de valeur dans le programme.", lastIdToken);
        exit(EXIT_FAILURE);
    }
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    EXPR();
    // Stocker la valeur au sommet de la pile dans l'adresse indiquée dans le sous-sommet
    GENERER1(STO);
}

void SI()
{
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    IND_BZE = PC;
    GENERER2(BZE, PC + 1);
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    INST();
    if (SYM_COUR.CODE == ELSE_TOKEN)
    {
        INST();
    }
}

void TANTQUE()
{
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    LABEL_BRN = PC + 1;
    COND();
    INDICE_BZE = PC;
    GENERER2(BZE, INDICE_BZE);
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
    GENERER2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].SUITE = PC + 1;
}

void ECRIRE()
{
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    GENERER1(PRN);
    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        EXPR();
        GENERER1(PRN);
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void LIRE()
{
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    
    // Définir une nouvelle variable en mémoire
    strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
    TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
    TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
	
	// Charger l'adresse de cette variable au sommet de la pile
	GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
	// Lecture d'un entier, puis le stocker dans l'adresse du sommet de la pile
	GENERER1(INN);
    
    Test_Symbole(ID_TOKEN, ID_ERR);
    
    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        
        // Définir une nouvelle variable en mémoire
    	strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
    	TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
    	TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
    	
        // Charger l'adresse de cette variable au sommet de la pile
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
        // Lecture d'un entier, puis le stocker dans l'adresse du sommet de la pile
		GENERER1(INN);
		
        Test_Symbole(ID_TOKEN, ID_ERR);
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void COND()
{
    EXPR();
    RELOP();
    EXPR();
    switch(operation) {
    	case 1:
    		GENERER1(EQL);
    		break;
    	case 2:
    		GENERER1(NEQ);
    		break;
    	case 3:
    		GENERER1(LSS);
    		break;
    	case 4:
    		GENERER1(GTR);
    		break;
    	case 5:
    		GENERER1(LEQ);
    		break;
    	case 6:
    		GENERER1(GEQ);
    		break;
    	case default:
    		Erreur(ERREUR_ERR);
    		break;
	}
}

void EXPR()
{
    // TERM { ADDOP TERM }
    TERM();
    while (SYM_COUR.CODE == PLUS_TOKEN || SYM_COUR.CODE == MOINS_TOKEN)
    {
        ADDOP();
        TERM();
    }
}

void TERM()
{
    FACT();

    while (SYM_COUR.CODE == MULT_TOKEN || SYM_COUR.CODE == DIV_TOKEN)
    {
        MULOP();
        FACT();
    }
}

void FACT()
{
    switch (SYM_COUR.CODE)
    {
    case ID_TOKEN:
    	// Itérer sur la table des symboles pour trouver une correspondance des noms
    	int i;
        for(int i = 0; i < IND_DER_SYM_ACC; i++) {
        	if(strcmp(TABLESYM[i].NOM, SYM_COUR.NOM) == 0) {
        		// Empiler l'adresse de la constante ou de la variable trouvée
        		GENERER2(LDA, TABLESYM[i].ADRESSE);
        		// Remplace cette adresse par sa valeur
        		GENERER1(LDV);
        		break;
			}
		}
		
        Test_Symbole(ID_TOKEN, ID_ERR);
        
        break;
    case NUM_TOKEN:
    	// Empiler la valeur trouvée
    	GENERER2(LDI, SYM_COUR.val);
    	
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case PO_TOKEN:
        Test_Symbole(PO_TOKEN, PO_ERR);
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void RELOP()
{
    switch (SYM_COUR.CODE)
    {
    case EG_TOKEN:
    	Test_Symbole(EG_TOKEN, EG_ERR);
    	operation = 1;
    	break;
    case DIFF_TOKEN:
    	Test_Symbole(DIFF_TOKEN, DIFF_ERR);
    	operation = 2;
    	break;
     case INF_TOKEN:
        Test_Symbole(INF_TOKEN, INF_ERR);
        operation = 3;
        break;
    case SUP_TOKEN:
        Test_Symbole(SUP_TOKEN, SUP_ERR);
        operation = 4;
        break;
    case INFEG_TOKEN:
        Test_Symbole(INFEG_TOKEN, INFEG_ERR);
        operation = 5;
        break;
    case SUPEG_TOKEN:
        Test_Symbole(SUPEG_TOKEN, SUPEG_ERR);
        operation = 6;   
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void ADDOP()
{
    switch (SYM_COUR.CODE)
    {
    case PLUS_TOKEN:
        Test_Symbole(PLUS_TOKEN, PLUS_ERR);
        GENERER1(ADD);
        break;
    case MOINS_TOKEN:
        Test_Symbole(MOINS_TOKEN, MOINS_ERR);
        GENERER1(SUB);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void MULOP()
{
    switch (SYM_COUR.CODE)
    {
    case MULT_TOKEN:
        Test_Symbole(MULT_TOKEN, MULT_ERR);
        GENERER1(MUL);
        break;
    case DIV_TOKEN:
        Test_Symbole(DIV_TOKEN, DIV_ERR);
        GENERER1(DIV);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void POUR()
{
    Test_Symbole(FOR_TOKEN, FOR_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
	
    switch (SYM_COUR.CODE)
    {
    case DOWNTO_TOKEN:
        Test_Symbole(DOWNTO_TOKEN, DOWNTO_ERR);
        break;
    case INTO_TOKEN:
        Test_Symbole(INTO_TOKEN, INTO_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }

    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
}

/*
REPEAT_TOKEN,UNTIL_TOKEN,FOR_TOKEN,ELSE_TOKEN,CASE_TOKEN,OF_TOKEN*/

void REPETER()
{
    Test_Symbole(REPEAT_TOKEN, REPEAT_ERR);
    INST();
    Test_Symbole(UNTIL_TOKEN, UNTIL_ERR);
    COND();
}

void CAS()
{
    Test_Symbole(CASE_TOKEN, CASE_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(OF_TOKEN, OF_TOKEN);
    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(DDOT_TOKEN, DDOT_ERR);
    INST();
    while (SYM_COUR.CODE == NUM_TOKEN)
    {
        Sym_Suiv();
        Test_Symbole(DDOT_TOKEN, DDOT_ERR);
        INST();
    }
    if (SYM_COUR.CODE == ELSE_TOKEN)
    {
        Sym_Suiv();
        INST();
    }

    Test_Symbole(END_TOKEN, END_ERR);
}
