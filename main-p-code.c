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
    DDOT_TOKEN,
    INTEGER_TOKEN,
    FLOAT_TOKEN,
    BOOLEAN_TOKEN,
    STRING_TOKEN,
    ARRAY_TOKEN,
    INTEGER_DATA_TOKEN,
    FLOAT_DATA_TOKEN,
    BOOLEAN_DATA_TOKEN,
    STRING_DATA_TOKEN,
    ARRAY_DATA_TOKEN,
    ERROR_TOKEN,
    QMARK_TOKEN,
    SQUARE_BRACKET_OPEN_TOKEN,
    SQUARE_BRACKET_CLOSE_TOKEN,
    ECRIRE_LN_TOKEN,
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
    DDOT_ERR,
    INST_PCODE_ERR,
    INTEGER_ERR,
    FLOAT_ERR,
    BOOLEAN_ERR,
    STRING_ERR,
    ARRAY_ERR,
    INTEGER_DATA_ERR,
    FLOAT_DATA_ERR,
    BOOLEAN_DATA_ERR,
    STRING_DATA_ERR,
    ARRAY_DATA_ERR,
    QMARK_ERR,
    SQUARE_BRACKET_OPEN_ERR,
    SQUARE_BRACKET_CLOSE_ERR,
    ECRIRE_LN_ERR,
} CODES_ERR;

int LOOP_LOGIC = 0;

void TYPE(int isIntitlized);

const char *getErrorMessage(CODES_ERR error_code);

const char *getErrorMessage(CODES_ERR error_code)
{
    switch (error_code)
    {
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
    case INTEGER_DATA_ERR:
        return "Integer format error";
    case FLOAT_DATA_ERR:
        return "Float format error";
    case BOOLEAN_DATA_ERR:
        return "Boolean format error";
    case STRING_DATA_ERR:
        return "String format error";
    case ARRAY_DATA_ERR:
        return "Array format error";
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
    case INTEGER_ERR:
        return "Integer error";
    case FLOAT_ERR:
        return "Float error";
    case BOOLEAN_ERR:
        return "Boolean error";
    case STRING_ERR:
        return "String error";
    case ARRAY_ERR:
        return "Array error";
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
    char NOM[255];
    int val;
} TSym_Cour;

TSym_Cour head;
TSym_Cour SYM_COUR;

FILE *fichier;

char Car_Cour; // caract�re courant

typedef enum
{
    TPROG,
    T_INTEGER_CONST,
    T_INTEGER_VAR,
    T_FLOAT_CONST,
    T_FLOAT_VAR,
    T_BOOLEAN_CONST,
    T_BOOLEAN_VAR,
    T_STRING_CONST,
    T_STRING_VAR,
    T_ARRAY_CONST,
    T_ARRAY_VAR
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

int MEM[TAILLEMEM]; // M�moire (Pile de la machine)
int SP;             // Pointeur vers le sommet du pile

typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    EQL,
    NEQ,
    GTR,
    LSS,
    GEQ,
    LEQ,
    PRN,
    INN,
    INT,
    LDI,
    LDA,
    LDV,
    STO,
    BRN,
    BZE,
    HLT
} MNEMONIQUES;

typedef struct
{
    MNEMONIQUES MNE; // Instru
    int SUITE;       // Son suivant
} INSTRUCTION;

INSTRUCTION PCODE[TAILLECODE];

int LABEL_BRN;
int INDICE_BZE;
int IND_BZE;
int INDICE_BRN;

int PC = 0; // Compteur d'instructions

int opRELOP = 0;
int opMULOP = 0;
int opADDOP = 0;
int opLoop = 0;

// Prototypes des fonctions � utiliser
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
void Erreur(CODES_ERR code, char *origin);
void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR);
void PROGRAM();
void BLOCK();
void CONSTS();
void Sym_Suiv();
void lire_mot();
void lire_string();
void lire_nombre();
void CAS();
void POUR();
void REPETER();

void Check();

// P CODE section
void GENERER1(MNEMONIQUES M);
void GENERER2(MNEMONIQUES M, int A);
void SaveInstToFile(FILE *FICH_SORTIE, INSTRUCTION INST, int i);
void INTER_PCODE();
void INTER_INST(INSTRUCTION INST);
void INTER_PCODE();

// Definition des fonctions � utiliser

void lire_string()
{

    // taile de la chaine
    char mot[255];
    int indice = 0;

    // Lecture du premier caract�re (lettre)
    mot[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des caract�res suivants (lettres ou chiffres)

    while (Car_Cour != '"')
    {
        mot[indice++] = Car_Cour;
        Lire_Car();
    }

    // Ajout du caract�re de fin de cha�ne
    mot[indice] = '\0';

    SYM_COUR.CODE = STRING_DATA_TOKEN;

    // Stockage du mot dans le jeton
    strcpy(SYM_COUR.NOM, mot);
    // printf("STRING 2: %s\n", SYM_COUR.NOM);
}

void lire_mot()
{
    char mot[20];
    int indice = 0;

    // Lecture du premier caract�re (lettre)
    mot[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des caract�res suivants (lettres ou chiffres)
    while (isalpha(Car_Cour) || isdigit(Car_Cour))
    {
        mot[indice++] = Car_Cour;
        Lire_Car();
    }

    // Ajout du caract�re de fin de cha�ne
    mot[indice] = '\0';

    // V�rifier si le mot est un mot-cl�
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
    else if (stricmp(mot, "writeln") == 0)
    {
        SYM_COUR.CODE = ECRIRE_LN_TOKEN;
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
    else if (stricmp(mot, "to") == 0)
    {
        SYM_COUR.CODE = INTO_TOKEN;
    }
    else if (stricmp(mot, "downto") == 0)
    {
        SYM_COUR.CODE = DOWNTO_TOKEN;
    }
    else if (stricmp(mot, "integer") == 0)
    {
        SYM_COUR.CODE = INTEGER_TOKEN;
    }
    else if (stricmp(mot, "float") == 0)
    {
        SYM_COUR.CODE = FLOAT_TOKEN; // 1
    }
    else if (stricmp(mot, "boolean") == 0)
    {
        SYM_COUR.CODE = BOOLEAN_TOKEN; // 2
    }
    else if (stricmp(mot, "string") == 0)
    {
        SYM_COUR.CODE = STRING_TOKEN; // 3
    }
    else if (stricmp(mot, "array") == 0)
    {
        SYM_COUR.CODE = ARRAY_TOKEN; // 4
    }
    else if (stricmp(mot, "true") == 0)
    {
        SYM_COUR.CODE = BOOLEAN_DATA_TOKEN;
        SYM_COUR.val = 1;
    }
    else if (stricmp(mot, "false") == 0)
    {
        SYM_COUR.CODE = BOOLEAN_DATA_TOKEN;
        SYM_COUR.val = 0;
    }
    else
    {
        // Si ce n'est pas un mot-cl�, c'est un identifiant
        SYM_COUR.CODE = ID_TOKEN;
        // Rep�rer les ID_TOKEN par des adresses
        // TABLESYM[IND_DER_SYM_ACC].NOM = SYM_COUR.NOM;
        // TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
        // TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
        // IND_DER_SYM_ACC++;
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
    char nombre[32];
    int indice = 0;
    int isFloat = 0; // Variable pour indiquer si le nombre est un flottant

    // Lecture du premier chiffre
    nombre[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des chiffres suivants
    while (isdigit(Car_Cour) || Car_Cour == '.')
    {
        if (Car_Cour == '.')
        {
            isFloat = 1; // Le nombre contient un point, donc c'est un flottant
        }
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }

    // Ajout du caractère de fin de chaîne
    nombre[indice] = '\0';

    // Stockage du nombre dans le jeton
    if (isFloat)
    {
        SYM_COUR.CODE = FLOAT_DATA_TOKEN;
        SYM_COUR.val = atof(nombre);
    }
    else
    {
        SYM_COUR.CODE = INTEGER_DATA_TOKEN;
        SYM_COUR.val = atoi(nombre);
    }

    strcpy(SYM_COUR.NOM, nombre);
}

void TYPE(int isIntitlized)
{
    Test_Symbole(DDOT_TOKEN, DDOT_ERR);
    switch (SYM_COUR.CODE)
    {
    case FLOAT_TOKEN:
        Sym_Suiv();
        if (isIntitlized)
        {
            Test_Symbole(EG_TOKEN, EG_ERR);

            // Empiler la valeur lue (correspond a NUM_TOKEN)
            GENERER2(LDI, SYM_COUR.val);
            GENERER1(STO);

            IND_DER_SYM_ACC++;

            Test_Symbole(FLOAT_DATA_TOKEN, FLOAT_DATA_ERR);

            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;
    case INTEGER_TOKEN:
        Sym_Suiv();
        if (isIntitlized)
        {
            Test_Symbole(EG_TOKEN, EG_ERR);

            // Empiler la valeur lue (correspond a NUM_TOKEN)
            GENERER2(LDI, SYM_COUR.val);
            GENERER1(STO);

            IND_DER_SYM_ACC++;

            Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);

            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;
    case BOOLEAN_TOKEN:
        Sym_Suiv();
        if (isIntitlized)
        {
            Test_Symbole(EG_TOKEN, EG_ERR);
            Test_Symbole(BOOLEAN_DATA_TOKEN, BOOLEAN_DATA_ERR);
            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;
    case STRING_TOKEN:
        Sym_Suiv();

        if (isIntitlized)
        {
            Test_Symbole(EG_TOKEN, EG_ERR);

            lire_string();
            Test_Symbole(STRING_DATA_TOKEN, STRING_DATA_ERR);

            Test_Symbole(QMARK_TOKEN, QMARK_ERR);
            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;
    case ARRAY_TOKEN:
        Sym_Suiv();
        if (isIntitlized)
        {
            Test_Symbole(EG_TOKEN, EG_ERR);

            Test_Symbole(SQUARE_BRACKET_OPEN_TOKEN, SQUARE_BRACKET_OPEN_ERR);
            Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);
            while (SYM_COUR.CODE == VIR_TOKEN)
            {
                Sym_Suiv();
                Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);
            }
            Test_Symbole(SQUARE_BRACKET_CLOSE_TOKEN, SQUARE_BRACKET_CLOSE_ERR);
            
            //Test_Symbole(ARRAY_DATA_TOKEN, ARRAY_DATA_ERR);
            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;

    default:
        Erreur(VAR_BEGIN_ERR, "VARS");
        break;
    }
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
            TAB_IDFS[k].TIDF = T_INTEGER_CONST;
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
            TAB_IDFS[k].TIDF = T_INTEGER_VAR;
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
            if (T_INTEGER_CONST == TAB_IDFS[i].TIDF)
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
        case '#':
            Lire_Car();
            while (Car_Cour != '\n')
            {
                Lire_Car();
            }
            Lire_Car();
            Sym_Suiv();
            break;
        case '$':
            Lire_Car();
            while (Car_Cour != '$')
            {
                Lire_Car();
            }
            Lire_Car();
            Sym_Suiv();
            break;
            
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
        case '"':
            SYM_COUR.CODE = QMARK_TOKEN;
            Lire_Car();
            break;
        case '[':
            SYM_COUR.CODE = SQUARE_BRACKET_OPEN_TOKEN;
            Lire_Car();
            break;
        case ']':
            SYM_COUR.CODE = SQUARE_BRACKET_CLOSE_TOKEN;
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

void Erreur(CODES_ERR code, char *origin)
{
    // printf("Syntaxic error\n");
    printf("Syntaxic error: %s  + origin: %s\n", getErrorMessage(code), origin);
    // printf("Current Token: %d\n", SYM_COUR.CODE);
    // printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

// Fonctions de g�n�ration du P-CODE

void GENERER1(MNEMONIQUES M)
{
    if (PC == TAILLECODE)
    {
        exit(EXIT_FAILURE);
    }
    PC++;
    PCODE[PC].MNE = M;
}

void GENERER2(MNEMONIQUES M, int A)
{
    if (PC == TAILLECODE)
    {
        exit(EXIT_FAILURE);
    }
    PC++;
    PCODE[PC].MNE = M;
    PCODE[PC].SUITE = A;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interpreteur

void SaveInstToFile(FILE *FICH_SORTIE, INSTRUCTION INST, int i)
{
    switch (INST.MNE)
    {
    case LDA:
        fprintf(FICH_SORTIE, "%s \t %d \n", "LDA", INST.SUITE);
        break;
    case LDI:
        fprintf(FICH_SORTIE, "%s \t %d \n", "LDI", INST.SUITE);
        break;
    case INT:
        fprintf(FICH_SORTIE, "%s \t %d \n", "INT", INST.SUITE);
        break;
    case BZE:
        fprintf(FICH_SORTIE, "%s \t %d \n", "BZE", INST.SUITE);
        break;
    case BRN:
        fprintf(FICH_SORTIE, "%s \t %d \n", "BRN", INST.SUITE);
        break;
    case LDV:
        fprintf(FICH_SORTIE, "%s \n", "LDV");
        break;
    case ADD:
        fprintf(FICH_SORTIE, "%s \n", "ADD");
        break;
    case SUB:
        fprintf(FICH_SORTIE, "%s \n", "SUB");
        break;
    case MUL:
        fprintf(FICH_SORTIE, "%s \n", "MUL");
        break;
    case DIV:
        fprintf(FICH_SORTIE, "%s \n", "DIV");
        break;
    case LEQ:
        fprintf(FICH_SORTIE, "%s \n", "LEQ");
        break;
    case GEQ:
        fprintf(FICH_SORTIE, "%s \n", "GEQ");
        break;
    case NEQ:
        fprintf(FICH_SORTIE, "%s \n", "NEQ");
        break;
    case LSS:
        fprintf(FICH_SORTIE, "%s \n", "LSS");
        break;
    case GTR:
        fprintf(FICH_SORTIE, "%s \n", "GTR");
        break;
    case EQL:
        fprintf(FICH_SORTIE, "%s \n", "EQL");
        break;
    case HLT:
        fprintf(FICH_SORTIE, "%s \n", "HLT");
        break;
    case STO:
        fprintf(FICH_SORTIE, "%s \n", "STO");
        break;
    case INN:
        fprintf(FICH_SORTIE, "%s \n", "INN");
        break;
    case PRN:
        fprintf(FICH_SORTIE, "%s \n", "PRN");
        break;

    default:
        Erreur(INST_PCODE_ERR, "SaveInstToFile");
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void INTER_PCODE()
{
    PC = 0;
    while (PCODE[PC].MNE != HLT)
        INTER_INST(PCODE[PC]);
}

void INTER_INST(INSTRUCTION INST)
{
    int val1, adr, val2;
    switch (INST.MNE)
    {
    case INT:
        OFFSET = SP = INST.SUITE;
        PC++;
        break;
    case LDI:
        MEM[++SP] = INST.SUITE;
        PC++;
        break;
    case LDA:
        MEM[++SP] = INST.SUITE;
        PC++;
        break;
    case STO:
        val1 = MEM[SP--];
        adr = MEM[SP--];
        MEM[adr] = val1;
        PC++;
        break;
    case LDV:
        adr = MEM[SP--];
        MEM[++SP] = MEM[adr];
        PC++;
        break;
    case EQL:
        val1 = MEM[SP--];
        val2 = MEM[SP--];
        MEM[++SP] = (val1 == val2);
        PC++;
        break;
    case LEQ:
        val2 = MEM[SP--];
        val1 = MEM[SP--];
        MEM[++SP] = (val1 <= val2);
        PC++;
        break;
    case GEQ:
        val2 = MEM[SP--];
        val1 = MEM[SP--];
        MEM[++SP] = (val1 >= val2);
        PC++;
        break;
    case LSS:
        val2 = MEM[SP--];
        val1 = MEM[SP--];
        MEM[++SP] = (val1 < val2);
        PC++;
        break;
    case GTR:
        val2 = MEM[SP--];
        val1 = MEM[SP--];
        MEM[++SP] = (val1 > val2);
        PC++;
        break;
    case NEQ:
        val2 = MEM[SP--];
        val1 = MEM[SP--];
        MEM[++SP] = (val1 != val2);
        PC++;
        break;
    case INN:
        scanf("%d", &val1);
        adr = MEM[SP--];
        MEM[adr] = val1;
        PC++;
        break;

    case BZE:
        if (MEM[SP--] == 0)
            PC = INST.SUITE;
        else
            PC++;
        break;
    case BRN:
        PC = INST.SUITE;
        break;
    case HLT:
        PC++;
        break;
    case ADD:
        val1 = MEM[SP--];
        val2 = MEM[SP--];
        MEM[++SP] = val1 + val2;
        PC++;
        break;
    case SUB:
        val1 = MEM[SP--];
        val2 = MEM[SP--];
        MEM[++SP] = val1 - val2;
        PC++;
        break;
    case MUL:
        val1 = MEM[SP--];
        val2 = MEM[SP--];
        MEM[++SP] = val1 * val2;
        PC++;
        break;
    case DIV:
        val1 = MEM[SP--];
        val2 = MEM[SP--];
        MEM[++SP] = val1 / val2;
        PC++;
        break;
    case PRN:
        printf("%d\n", MEM[SP--]);
        PC++;
        break;
    }

    
    // int i;
    // for (i = 0; i < SP; i++)
    // {
    //     printf("[%d | ", MEM[i]);
    // }

    // printf("\n\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR)
{
    if (SYM_COUR.CODE == cl)
    {
        Sym_Suiv();
    }
    else
        Erreur(COD_ERR, "Test_Symbole");
}

void PROGRAM()
{
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    current_region = RPROG;
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    GENERER1(HLT);
    Test_Symbole(PT_TOKEN, PT_ERR);

    // for (int i = 0; i < TABLEINDEX; i++)
    // {
    //     if (strcmp(TABLESYM[i].NOM, SYM_COUR.NOM) == 0)
    //     {
    //         // Empiler l'adresse de la constante ou de la variable trouv�e
    //         GENERER2(LDA, TABLESYM[i].ADRESSE);
    //         // Remplace cette adresse par sa valeur
    //         GENERER1(LDV);

    //         printf("ID: %s\n", SYM_COUR.NOM);
    //         break;
    //     }
    //     printf("ID inside table: %s - %d\n", TABLESYM[i].NOM, TABLESYM[i].CLASSE);
    // }
}

void BLOCK()
{
    current_region = RCONST;
    CONSTS();
    current_region = RVAR;
    VARS();
    current_region = RBEGIN;
    PCODE[0].MNE = INT;
    PCODE[0].SUITE = 5;
    INSTS();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CONSTS()
{
    switch (SYM_COUR.CODE)
    {
    case CONST_TOKEN:
        Sym_Suiv();

        // Definir une nouvelle constante en memoire
        TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
        strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
        TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;

        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);

        Test_Symbole(ID_TOKEN, ID_ERR);
        TYPE(1);
        while (SYM_COUR.CODE == ID_TOKEN)
        {
            // Definir une nouvelle constante en memoire
            TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
            strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
            TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);

            Sym_Suiv();

            TYPE(1);
        };
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR, "CONSTS");
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
        TYPE(0);

        while (SYM_COUR.CODE == VIR_TOKEN)
        {
            Sym_Suiv();
            Test_Symbole(ID_TOKEN, ID_ERR);
            TYPE(0);
        }

        Test_Symbole(PV_TOKEN, PV_ERR);
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(VAR_BEGIN_ERR, "VARS");
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
            Erreur(FIN_ERR, "INSTS");
        }
    }
    else
    {
        Erreur(BEGIN_ERR, "INSTS");
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
    case ECRIRE_LN_TOKEN:   
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
    int i;
    int exists = 0;
    for (i = 0; i < TABLEINDEX; i++)
    {
        if (strcmp(TABLESYM[i].NOM, SYM_COUR.NOM) == 0)
        {
            // Empiler l'adresse de la constante ou de la variable trouv�e
            GENERER2(LDA, TABLESYM[i].ADRESSE);
            exists = 1;
            break;
        }
    }

    if (!exists)
    {

        // D�finir une nouvelle variable en m�moire
        strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
        TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
        TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
        // Empiler l'adresse de cette nouvelle variable pour but d'affectation (Voir FACT())
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);

        IND_DER_SYM_ACC++;
    }

    // Remplace cette adresse par sa valeur
    // GENERER1(LDV);

    // ID := EXPR
    Test_Symbole(ID_TOKEN, ID_ERR);
    if (T_INTEGER_VAR != lastType)
    {
        printf("%s ----> Erreur:  Une constante ne peut changer de valeur dans le programme.", lastIdToken);
        exit(EXIT_FAILURE);
    }
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    EXPR();
    // Stocker la valeur au sommet de la pile dans l'adresse indiqu�e dans le sous-sommet
    GENERER1(STO);
}

void SI()
{
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    GENERER1(BZE);
    IND_BZE = PC;
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    INST();
    GENERER1(BRN);
    INDICE_BRN = PC;
    PCODE[IND_BZE].SUITE = PC + 1;
    if (SYM_COUR.CODE == ELSE_TOKEN)
    {
        Sym_Suiv();
        INST();
        PCODE[INDICE_BRN].SUITE = PC + 1;
    }
}

void TANTQUE()
{
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    LABEL_BRN = PC + 1;
    COND();
    GENERER1(BZE); // Branchement pour d�passer les instructions de IF, si la condition est fausse (sommet = 0)
    INDICE_BZE = PC;
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
    GENERER2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].SUITE = PC + 1;
}

void ECRIRE()
{
    Sym_Suiv();
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

    int i;
    int exists = 0;
    for (i = 0; i < TABLEINDEX; i++)
    {
        if (strcmp(TABLESYM[i].NOM, SYM_COUR.NOM) == 0)
        {
            // Empiler l'adresse de la constante ou de la variable trouv�e
            GENERER2(LDA, TABLESYM[i].ADRESSE);
            exists = 1;
            break;
        }
    }

    if (exists == 0)
    {
        // D�finir une nouvelle variable en m�moire
        strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
        TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
        TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;

        // Charger l'adresse de cette variable au sommet de la pile
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);

        IND_DER_SYM_ACC++;
    }

    // Lecture d'un entier, puis le stocker dans l'adresse du sommet de la pile
    GENERER1(INN);

    Test_Symbole(ID_TOKEN, ID_ERR);

    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();

        int i;
        int exists = 0;
        for (i = 0; i < TABLEINDEX; i++)
        {
            if (strcmp(TABLESYM[i].NOM, SYM_COUR.NOM) == 0)
            {
                // Empiler l'adresse de la constante ou de la variable trouv�e
                GENERER2(LDA, TABLESYM[i].ADRESSE);
                exists = 1;
                break;
            }
        }

        if (exists == 0)
        {
            // D�finir une nouvelle variable en m�moire
            strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
            TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
            TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;

            // Charger l'adresse de cette variable au sommet de la pile
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);

            IND_DER_SYM_ACC++;
        }
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
    switch (opRELOP)
    {
    case 1:
        if (LOOP_LOGIC)
        {
            GENERER1(NEQ);
            LOOP_LOGIC = 0;
        }
        else
        {
            GENERER1(EQL);
        }
        break;
    case 2:
        if (LOOP_LOGIC)
        {
            GENERER1(EQL);
            LOOP_LOGIC = 0;
        }
        else
        {
            GENERER1(NEQ);
        }
        break;
    case 3:
        if (LOOP_LOGIC)
        {
            GENERER1(GEQ);
            LOOP_LOGIC = 0;
        }
        else
        {
            GENERER1(LSS);
        }
        break;
    case 4:
        if (LOOP_LOGIC)
        {
            GENERER1(LEQ);
            LOOP_LOGIC = 0;
        }
        else
        {
            GENERER1(GTR);
        }
        break;
    case 5:
        if (LOOP_LOGIC)
        {
            GENERER1(GTR);
            LOOP_LOGIC = 0;
        }
        else
        {
            GENERER1(LEQ);
        }
        break;
    case 6:
        if (LOOP_LOGIC)
        {
            GENERER1(LSS);
            LOOP_LOGIC = 0;
        }
        else
        {
            GENERER1(GEQ);
        }
        break;
    default:
        Erreur(ERREUR_ERR, "COND");
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
        switch (opADDOP)
        {
        case 1:
            GENERER1(ADD);
            break;
        case 2:
            GENERER1(SUB);
            break;
        default:
            Erreur(ERREUR_ERR, "EXPR");
            break;
        }
    }
}

void TERM()
{
    FACT();

    while (SYM_COUR.CODE == MULT_TOKEN || SYM_COUR.CODE == DIV_TOKEN)
    {
        MULOP();
        FACT();
        switch (opMULOP)
        {
        case 1:
            GENERER1(MUL);
            break;
        case 2:
            GENERER1(DIV);
            break;
        default:
            Erreur(ERREUR_ERR, "TERM");
            break;
        }
    }
}

void FACT()
{
    // It�rer sur la table des symboles pour trouver une correspondance des noms
    int i;
    switch (SYM_COUR.CODE)
    {
    case ID_TOKEN:

        for (i = 0; i < TABLEINDEX; i++)
        {
            if (strcmp(TABLESYM[i].NOM, SYM_COUR.NOM) == 0)
            {
                // Empiler l'adresse de la constante ou de la variable trouv�e
                GENERER2(LDA, TABLESYM[i].ADRESSE);
                // Remplace cette adresse par sa valeur
                GENERER1(LDV);

                // printf("ID: %s\n", SYM_COUR.NOM);
                break;
            }
        }

        Test_Symbole(ID_TOKEN, ID_ERR);

        break;
    case INTEGER_DATA_TOKEN:
        // Empiler la valeur trouv�e
        GENERER2(LDI, SYM_COUR.val);

        Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);
        break;
    case PO_TOKEN:
        Test_Symbole(PO_TOKEN, PO_ERR);
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;
    default:
        Erreur(ERREUR_ERR, "FACT");
        break;
    }
}

void RELOP()
{
    switch (SYM_COUR.CODE)
    {
    case EG_TOKEN:
        Test_Symbole(EG_TOKEN, EG_ERR);
        opRELOP = 1;
        break;
    case DIFF_TOKEN:
        Test_Symbole(DIFF_TOKEN, DIFF_ERR);
        opRELOP = 2;
        break;
    case INF_TOKEN:
        Test_Symbole(INF_TOKEN, INF_ERR);
        opRELOP = 3;
        break;
    case SUP_TOKEN:
        Test_Symbole(SUP_TOKEN, SUP_ERR);
        opRELOP = 4;
        break;
    case INFEG_TOKEN:
        Test_Symbole(INFEG_TOKEN, INFEG_ERR);
        opRELOP = 5;
        break;
    case SUPEG_TOKEN:
        Test_Symbole(SUPEG_TOKEN, SUPEG_ERR);
        opRELOP = 6;
        break;
    default:
        Erreur(ERREUR_ERR, "RELOP");
        break;
    }
}

void ADDOP()
{
    switch (SYM_COUR.CODE)
    {
    case PLUS_TOKEN:
        Test_Symbole(PLUS_TOKEN, PLUS_ERR);
        opADDOP = 1;
        break;
    case MOINS_TOKEN:
        Test_Symbole(MOINS_TOKEN, MOINS_ERR);
        opADDOP = 2;
        break;
    default:
        Erreur(ERREUR_ERR, "ADDOP");
        break;
    }
}

void MULOP()
{
    switch (SYM_COUR.CODE)
    {
    case MULT_TOKEN:
        Test_Symbole(MULT_TOKEN, MULT_ERR);
        opMULOP = 1;
        break;
    case DIV_TOKEN:
        Test_Symbole(DIV_TOKEN, DIV_ERR);
        opMULOP = 2;
        break;
    default:
        Erreur(ERREUR_ERR, "MULOP");
        break;
    }
}

void POUR()
{
    Test_Symbole(FOR_TOKEN, FOR_ERR);

    // D�finir une nouvelle variable en m�moire
    strcpy(TABLESYM[IND_DER_SYM_ACC].NOM, SYM_COUR.NOM);
    TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
    int offset = ++OFFSET;
    TABLESYM[IND_DER_SYM_ACC].ADRESSE = offset;

    IND_DER_SYM_ACC++;

    // Empiler l'adresse de cette nouvelle variable pour but d'affectation (Voir FACT())
    GENERER2(LDA, offset);

    // ID := EXPR
    Test_Symbole(ID_TOKEN, ID_ERR);
    if (T_INTEGER_VAR != lastType)
    {
        printf("%s ----> Erreur:  Une constante ne peut changer de valeur dans le programme.", lastIdToken);
        exit(EXIT_FAILURE);
    }
    Test_Symbole(AFF_TOKEN, AFF_ERR);

    // Stockage de la valeur initiale
    GENERER2(LDI, SYM_COUR.val);
    GENERER1(STO);

    Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);

    switch (SYM_COUR.CODE)
    {
    case DOWNTO_TOKEN:
        Sym_Suiv();
        opLoop = 1;
        break;
    case INTO_TOKEN:
        Sym_Suiv();
        opLoop = 2;
        break;
    default:
        Erreur(ERREUR_ERR, "POUR");
        break;
    }

    LABEL_BRN = PC + 1;

    GENERER2(LDA, offset);
    GENERER1(LDV);
    GENERER2(LDI, SYM_COUR.val);
    if (opLoop == 1)
    {
        GENERER2(LDI, -1);
        GENERER1(ADD);
    }
    else if (opLoop == 2)
    {
        GENERER2(LDI, 1);
        GENERER1(ADD);
    }
    GENERER1(NEQ);

    Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);

    GENERER1(BZE);
    INDICE_BZE = PC;

    Test_Symbole(DO_TOKEN, DO_ERR);

    INST();

    GENERER2(LDA, offset);
    GENERER2(LDA, offset); // Double LDA car LDV ecrase l'adresse et le remplace par sa valeur
    GENERER1(LDV);
    // GENERER2(LDI, 1);

    if (opLoop == 1)
    {
        GENERER2(LDI, -1);
        GENERER1(ADD);
    }
    else if (opLoop == 2)
    {
        GENERER2(LDI, 1);
        GENERER1(ADD);
    }
    GENERER1(STO);
    GENERER2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].SUITE = PC + 1;
}

/*

REPEAT_TOKEN,UNTIL_TOKEN,FOR_TOKEN,ELSE_TOKEN,CASE_TOKEN,OF_TOKEN

*/

void REPETER()
{
    LOOP_LOGIC = 1;
    Test_Symbole(REPEAT_TOKEN, REPEAT_ERR);
    LABEL_BRN = PC + 1;
    INST();
    // printf("%d\n", SYM_COUR.CODE);
    Test_Symbole(UNTIL_TOKEN, UNTIL_ERR);
    COND();
    GENERER1(BZE);
    INDICE_BZE = PC;
    GENERER2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].SUITE = PC + 1;
}

void CAS()
{
    Test_Symbole(CASE_TOKEN, CASE_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(OF_TOKEN, OF_TOKEN);
    Test_Symbole(INTEGER_DATA_TOKEN, INTEGER_DATA_ERR);
    Test_Symbole(DDOT_TOKEN, DDOT_ERR);
    INST();
    while (SYM_COUR.CODE == INTEGER_DATA_TOKEN)
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

void SavePCodeToFile(FILE *FICH_SORTIE);

void SavePCodeToFile(FILE *FICH_SORTIE)
{
    int i;
    for (i = 0; i <= PC; i++)
    {
        SaveInstToFile(FICH_SORTIE, PCODE[i], i);
    }
}

int main()
{
    FILE *FICH_SORTIE;
    FICH_SORTIE = fopen("pcode.txt", "w");
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
        perror("Erreur lors de l'allocation de memoire pour TAB_IDFS!");
        return 1;
    }

    Lire_Car();
    Sym_Suiv();

    PROGRAM();

    printf("Execution du programme faite.\n");

    INTER_PCODE();

    SavePCodeToFile(FICH_SORTIE);
    fclose(FICH_SORTIE);

    fclose(fichier);
    free(TAB_IDFS); // Free allocated memory

    return 0;
}
