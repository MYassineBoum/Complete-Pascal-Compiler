#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

const char* getErrorMessage(CODES_ERR error_code);

const char* getErrorMessage(CODES_ERR error_code) {
    switch(error_code) {
        case ID_ERR:
            return "Identifier error";
        case PROGRAM_ERR:
            return "Program error";
        case CONST_ERR:
            return "Constant error";
        case VAR_ERR:
            return "Variable error";
        case BEGIN_ERR:
            return "Begin error";
        case END_ERR:
            return "End error";
        case IF_ERR:
            return "If error";
        case THEN_ERR:
            return "Then error";
        case WHILE_ERR:
            return "While error";
        case DO_ERR:
            return "Do error";
        case READ_ERR:
            return "Read error";
        case WRITE_ERR:
            return "Write error";
        case PV_ERR:
            return "Semicolon error";
        case PT_ERR:
            return "Period error";
        case PLUS_ERR:
            return "Plus error";
        case MOINS_ERR:
            return "Minus error";
        case MULT_ERR:
            return "Multiplication error";
        case DIV_ERR:
            return "Division error";
        case VIR_ERR:
            return "Comma error";
        case AFF_ERR:
            return "Assignment error";
        case INF_ERR:
            return "Less than error";
        case INFEG_ERR:
            return "Less than or equal to error";
        case SUP_ERR:
            return "Greater than error";
        case SUPEG_ERR:
            return "Greater than or equal to error";
        case DIFF_ERR:
            return "Not equal to error";
        case PO_ERR:
            return "Opening parenthesis error";
        case PF_ERR:
            return "Closing parenthesis error";
        case FIN_ERR:
            return "End of file error";
        case NUM_ERR:
            return "Number format error";
        case ERREUR_ERR:
            return "General error";
        case EOF_ERR:
            return "End of file error";
        case EG_ERR:
            return "Equality error";
        case CONST_VAR_BEGIN_ERR:
            return "Constant/Variable/Begin declaration error";
        case VAR_BEGIN_ERR:
            return "Variable/Begin declaration error";
        case REPEAT_ERR:
            return "Repeat error";
        case UNTIL_ERR:
            return "Until error";
        case FOR_ERR:
            return "For error";
        case ELSE_ERR:
            return "Else error";
        case CASE_ERR:
            return "Case error";
        case OF_ERR:
            return "Of error";
        case INTO_ERR:
            return "Into error";
        case DOWNTO_ERR:
            return "Downto error";
        case DDOT_ERR:
            return "Double dot error";
        default:
            return "Unknown error";
    }
}

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
void Erreur(CODES_ERR code, char * origin);
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
    else if (stricmp(mot, "case") == 0)
    {
        SYM_COUR.CODE = CASE_TOKEN;
    }
    else if (stricmp(mot, "of") == 0)
    {
        SYM_COUR.CODE = OF_TOKEN;
    }
    else if (stricmp(mot, "to") == 0)
    {
        SYM_COUR.CODE = INTO_TOKEN;
    }
    else if (stricmp(mot, "downto") == 0)
    {
        SYM_COUR.CODE = DOWNTO_TOKEN;
    }
    else
    {
        // Si ce n'est pas un mot-clé, c'est un identifiant
        SYM_COUR.CODE = ID_TOKEN;
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

void Erreur(CODES_ERR code, char * origin )
{
    //printf("Syntaxic error\n");
    printf("Syntaxic error: %s  + origin: %s\n", getErrorMessage(code),origin);
    //printf("Current Token: %d\n", SYM_COUR.CODE);
    //printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR)
{
    if (SYM_COUR.CODE == cl)
    {
        Sym_Suiv();
    }
    else
        Erreur(COD_ERR, SYM_COUR.NOM);
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

void CONSTS()
{
    switch (SYM_COUR.CODE)
    {
    case CONST_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
        Test_Symbole(EG_TOKEN, EG_ERR);
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        Test_Symbole(PV_TOKEN, PV_ERR);
        while (SYM_COUR.CODE == ID_TOKEN)
        {
            Sym_Suiv();
            Test_Symbole(EG_TOKEN, EG_ERR);
            Test_Symbole(NUM_TOKEN, NUM_ERR);
            Test_Symbole(PV_TOKEN, PV_ERR);
        };
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR,"CONSTS");
        break;
    }
}

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
        Erreur(VAR_BEGIN_ERR,"VARS");
        break;
    }
}

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
            Erreur(FIN_ERR,"INSTS");
        }
    }
    else
    {
        Erreur(BEGIN_ERR,"INSTS");
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
    // ID := EXPR
    Test_Symbole(ID_TOKEN, ID_ERR);
    if (TVAR != lastType)
    {
        printf("%s ----> Erreur:  Une constante ne peut changer de valeur dans le programme.", lastIdToken);
        exit(EXIT_FAILURE);
    }
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    
    EXPR();
}

void SI()
{
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
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
    COND();
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
}

void ECRIRE()
{
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();

    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        EXPR();
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void LIRE()
{
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);

    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void COND()
{
    EXPR();
    RELOP();
    EXPR();
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
        Test_Symbole(ID_TOKEN, ID_ERR);
        break;
    case NUM_TOKEN:
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case PO_TOKEN:
        Test_Symbole(PO_TOKEN, PO_ERR);
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;
    default:
        Erreur(ERREUR_ERR,"FACT");
        break;
    }
}

void RELOP()
{
    switch (SYM_COUR.CODE)
    {
    case EG_TOKEN:
    case DIFF_TOKEN:
    case INF_TOKEN:
    case SUP_TOKEN:
    case INFEG_TOKEN:
    case SUPEG_TOKEN:
        Test_Symbole(EG_TOKEN, EG_ERR);
        break;
    default:
        Erreur(ERREUR_ERR,"RELOP");
        break;
    }
}

void ADDOP()
{
    switch (SYM_COUR.CODE)
    {
    case PLUS_TOKEN:
        Test_Symbole(PLUS_TOKEN, PLUS_ERR);
        break;
    case MOINS_TOKEN:
        Test_Symbole(MOINS_TOKEN, MOINS_ERR);
        break;
    default:
        Erreur(ERREUR_ERR,"ADDOP");
        break;
    }
}

void MULOP()
{
    switch (SYM_COUR.CODE)
    {
    case MULT_TOKEN:
        Test_Symbole(MULT_TOKEN, MULT_ERR);
        break;
    case DIV_TOKEN:
        Test_Symbole(DIV_TOKEN, DIV_ERR);
        break;
    default:
        Erreur(ERREUR_ERR,"MULOP");
        break;
    }
}

void POUR()
{
    Test_Symbole(FOR_TOKEN, FOR_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    Test_Symbole(NUM_TOKEN, NUM_ERR);

    switch (SYM_COUR.CODE)
    {
    case DOWNTO_TOKEN:
        Test_Symbole(DOWNTO_TOKEN, DOWNTO_ERR);
        break;
    case INTO_TOKEN:
        Test_Symbole(INTO_TOKEN, INTO_ERR);
        break;
    default:
        Erreur(ERREUR_ERR,"POUR");
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
