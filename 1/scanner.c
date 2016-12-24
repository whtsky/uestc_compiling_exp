#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE 1024

enum Token {
    T_SYMBOL = 11,
    T_CONST,

    T_LEFT_BRACKET = 24,
    T_RIGHT_BRACKET,
    T_SEMICOLON,
    T_EOL,
    T_EOF
};

char *operator_table[] = {
    "+", "-", "*", "/",
    "<", "<=", ">", ">=",
    "==", "!=", ":="
}; // length = 11

char *reserved_table[] = {
    "", "begin", "end", "integer", "function",
    "read", "write", "if", "then", "else",
};

int is_operator(char *s) {
    for(int i=0; i<11; i++) {
        if (strcmp(operator_table[i], s) == 0) {
            return i + 13;
        }
    }
    return 0;
}

bool is_operator_token(char ch) {
    switch(ch) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
        case '=':
        case '>':
        case '!':
        case ':':
            return true;
        default:
            return false;
    }
}

int is_reserved_token(char *s) {
    for(int i=1; i<10; i++) {
        // Todo: Hard coded reserved table length
        if (strcmp(reserved_table[i], s) == 0) {
            return i;
        }
    }
    return 0;
}

int is_sep_token(char ch) {
    switch(ch) {
        case '(': return T_LEFT_BRACKET;
        case ')': return T_RIGHT_BRACKET;
        case ';': return T_SEMICOLON;
        default: return 0;
    }
}

FILE *error_file;
FILE *lex_file;
FILE *symtab1e_file;

void write_error(const char *s) {
    fprintf(stdout, "%s", s);
    fprintf(error_file, "%s", s);
}

void write_lex(const char *s) {
    fprintf(stdout, "%s", s);
    fprintf(lex_file, "%s", s);
}

void write_sym(const char *s) {
    fprintf(stdout, "%s", s);
    fprintf(symtab1e_file, "%s", s);
}

typedef struct {
    int id;
    char name[MAX_LINE];
    int lineno;
} SymbleToken;
typedef SymbleToken *Symble;

Symble symbles[MAX_LINE];

// Add a symble to SymbleTable. Return Symble ID.
Symble add_symble(char *s, int lineno) {
    for(int i=0; i<MAX_LINE; i++) {
        if (symbles[i] == NULL) {
            symbles[i] = malloc(sizeof(SymbleToken));
            symbles[i]->id = i+1;
            symbles[i]->lineno = lineno;
            strcpy(symbles[i]->name, s);
            return symbles[i];
        }
        if (strcmp(symbles[i]->name, s) == 0) {
            return symbles[i];
        }
    }
    return NULL;
}

void write_token(char *s, int lineno) {
//    printf("\nToken:\n%s\nEndToken\n", s);
    char tmp[MAX_LINE];
    int token;
//    printf("Token:\n%s\n", s);
    if ((token = is_reserved_token(s)) != 0) {
        sprintf(tmp, "line %2d: (%d, 0) 保留字： %s\n", lineno, token, s);
    } else if ((token = is_operator(s)) != 0) {
        sprintf(tmp, "line %2d: (%d, 0) 操作符： %s\n", lineno, token, s);
    } else {
        Symble t = add_symble(s, lineno);
        sprintf(tmp, "line %2d: (%d, %d) 标识符：%s\n", lineno, T_SYMBOL, t->lineno, s);
    }
    write_lex(tmp);
}

bool valid_token(char ch) {
    if (isalnum(ch)) {
        return true;
    }
    return ch == '_';
}

void analytics_line(char *line, int lineno) {
    char ch, tmp[MAX_LINE], token_string[MAX_LINE];
    int token;
    while((ch = *(line++)) != '\n') {
        if (isalpha(ch)) {
            // 符号
            int i = 0;
            token_string[i++] = ch;
            while(valid_token(*line)) {
                token_string[i++] = *(line++);
            }
            token_string[i] = '\0';
            write_token(token_string, lineno);
        } else if (isdigit(ch)) {
            // 数字
            int num = ch - '0';
            while(isdigit(*line)) {
                num = num * 10 + (*(line++) - '0');
            }
            sprintf(tmp, "line %2d: (%d, 0) 常数： %d\n", lineno, T_CONST, num);
            write_lex(tmp);
        } else if (is_operator_token(ch)) {
            int i = 0;
            token_string[i++] = ch;
            while(is_operator_token(*line)) {
                token_string[i++] = *(line++);
            }
            token_string[i] = '\0';
            if ((token = is_operator(token_string)) != 0) {
                sprintf(tmp, "line %2d: (%d, 0) 运算符 %s\n", lineno, token, token_string);
                write_lex(tmp);
            } else {
                sprintf(tmp, "line %2d: 词法错误： %c\n", lineno, ch);
                write_error(tmp);
            }
        } else if (((token = is_sep_token(ch)) != 0) || ch == ' ') {
            // 界符，空格
            if (token != 0) {
                sprintf(tmp, "line %2d: (%d, 0) 界符： %c\n", lineno, token, ch);
                write_lex(tmp);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Windows 下的换行符是 \n\r ，所以我们可以 ignore \r
            sprintf(tmp, "line %2d: (%d, 0) 行尾符： \n", lineno, T_EOL);
            write_lex(tmp);
            return;
        } else {
            sprintf(tmp, "line %2d: 词法错误： %c(ASCII: %d)\n", lineno, ch, ch);
            write_error(tmp);
        }
    }
}


int main(void){
    FILE *f = fopen("source.txt", "r");
    lex_file = fopen("lex.txt", "w");
    symtab1e_file = fopen("symtab1e.txt", "w");
    error_file = fopen("error.txt", "w");
    if (f == NULL || lex_file == NULL || symtab1e_file == NULL || error_file == NULL) {
        exit(EXIT_FAILURE);
    }
    write_lex("吴昊天-2015220201015\n");

    int lineno = 0;
    char *line = NULL;
    char tmp[MAX_LINE];    
    size_t len = 0;
    while(getline(&line, &len, f) != -1) {
        write_lex("\n");
        analytics_line(line, ++lineno);
    }
    sprintf(tmp, "\nline %2d: (%d, 0) 结尾符： #\n", ++lineno, T_EOF);
    write_lex(tmp);

    free(line);
    fclose(f);

    // Write symbles
    write_sym("符号表\n");
    write_sym("编号\t行号\t名称\n");
    for(int i=0; i < MAX_LINE; i++) {
        if(symbles[i] == NULL) {
            break;
        }
        sprintf(tmp, "%d\t%d\t%s\n", i+1, symbles[i]->lineno, symbles[i]->name);
        write_sym(tmp);
    }

    return 0;
}
