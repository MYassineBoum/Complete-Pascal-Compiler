#include <stdio.h>

#define TAILLEMEM 1000
#define TAILLECODE 1000

typedef enum {
    ADD, SUB, MUL, DIV, EQL, NEQ, GTR, LSS, GEQ, LEQ, PRN, INN, INT, LDI, LDA, LDV, STO, BRN, BZE, HLT, NUM
} CLASS_LEX;

typedef struct {
    CLASS_LEX MNE;
    int SUITE;
} INSTRUCTION;

int MEM[TAILLEMEM];
INSTRUCTION PCODE[TAILLECODE];
int SP = -1;
int PC = -1;
int OFFSET;

void ECRIRE1(CLASS_LEX M) {
    if (PC == TAILLECODE)
        printf("Erreur: TAILLECODE atteinte\n");
    PC++;
    PCODE[PC].MNE = M;
}

void ECRIRE2(CLASS_LEX M, int A) {
    if (PC == TAILLECODE)
        printf("Erreur: TAILLECODE atteinte\n");
    PC++;
    PCODE[PC].MNE = M;
    PCODE[PC].SUITE = A;
}

void INTER_INST(INSTRUCTION INST) {
    int val1, adr, val2;
    switch (INST.MNE) {
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
        case BZE:
            if (MEM[SP--] == 0)
                PC = INST.SUITE;
            else
                PC++;
            break;
        case BRN:
            PC = INST.SUITE;
            break;
        // Ajoutez les autres cas ici
    }
}

void INTER_PCODE() {
    PC = 0;
    while (PCODE[PC].MNE != HLT)
        INTER_INST(PCODE[PC]);
}

int main() {
    // hna i guess khes yt7et l p code 
    // Appelez l'interpréteur pour exécuter le programme
    INTER_PCODE();
    
    return 0;
}
