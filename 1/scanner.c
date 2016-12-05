#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

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
    char name[MAX_LINE];
    int lineno;
} *Symble;

Symble symbles[MAX_LINE];

void add_symble(char *s, int lineno) {
    for(int i=0; i<MAX_LINE; i++) {
        if (symbles[i] == NULL) {
            symbles[i] = malloc(sizeof(Symble));
            symbles[i]->lineno = lineno;
            strcpy(symbles[i]->name, s);
            return;
        }
        if (strcmp(symbles[i]->name, s) == 0) {
            return;
        }
    }
}

char *reserved_table[] = {
    "integer", "if", "then", "else",
    "begin", "end", "read", "write", "function"
};

void analytics_line(char *line, int lineno) {
    printf("%d: %s", lineno, line);
    char tmp[MAX_LINE];

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
    size_t len = 0;
    while(getline(&line, &len, f) != -1) {
        write_lex("\n");
        analytics_line(line, ++lineno);
    }
    free(line);
    fclose(f);
    
    char tmp[MAX_LINE];

    add_symble("233", 50);
    add_symble("233", 100);
    add_symble("yes!", 2);

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
