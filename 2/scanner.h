#ifndef UESTC_COMPILING_EXP_SCANNER_H
#define UESTC_COMPILING_EXP_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_LINE 1024

enum TokenID {
    T_SYMBOL = 11,
    T_CONST,
    T_ADD,
    T_MINUS,
    T_MUL,
    T_DIV,

    T_LEFT_BRACKET = 24,
    T_RIGHT_BRACKET,

    T_END
};

typedef struct {
    int id;
    char name[MAX_LINE];
} SymbleToken;
typedef SymbleToken *Symble;

typedef struct {
    int type;
    union {
        int v;
        char c;
        Symble s;
    } value;
} Token;


void E(void);
void E1(void);
void T(void);
void T1(void);
void F(void);
void A(void);
void M(void);

void next_token(void);

void write_error(const char *s);

void write_detail(const char *s);

void write_sym(const char *s);

#endif
