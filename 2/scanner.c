#include "scanner.h"

Symble symbles[MAX_LINE];

// Add a symble to SymbleTable. Return Symble ID.
Symble add_symble(char *s) {
    for(int i=0; i<MAX_LINE; i++) {
        if (symbles[i] == NULL) {
            symbles[i] = malloc(sizeof(SymbleToken));
            symbles[i]->id = i+1;
            strcpy(symbles[i]->name, s);
            return symbles[i];
        }
        if (strcmp(symbles[i]->name, s) == 0) {
            return symbles[i];
        }
    }
    return NULL;
}


Token sym;

char *processed;
char *waiting;
char *source;

void write_syntax_error(void) {
    char tmp[MAX_LINE];
    sprintf(tmp, "Error：%s(%s)%s， 不合法的语法\n", processed, waiting, source);
    write_error(tmp);
    exit(EXIT_FAILURE);
}

char *get_sym_string() {
    if (sym.type == T_SYMBOL) {
        return sym.value.s->name;
    }
    char *out;
    out = malloc(sizeof(char) * MAX_LINE);
    if (sym.type == T_END) {
        sprintf(out, "");
    } else if (sym.type == T_CONST) {
        sprintf(out, "%d", sym.value.v);
    } else {
        // sym.value is char
        sprintf(out, "%c", sym.value.c);
    }
    return out;

}

void print_current_status(void) {
    char tmp[MAX_LINE];
    sprintf(tmp, "已分析过的串：%s\n", processed);
    write_detail(tmp);
    sprintf(tmp, "当前分析：%s\n", waiting);
    write_detail(tmp);
    sprintf(tmp, "剩余串：%s\n\n", source);
    write_detail(tmp);
}

void advance(void) {
    if (waiting != NULL) {
        char old_processed[MAX_LINE];
        sprintf(old_processed, "%s", processed);
        sprintf(processed, "%s%s", old_processed, waiting);
    }
    next_token();
    waiting = get_sym_string();
}


void E(void) {
    write_detail("使用的产生式：E → TE’ \n");
    print_current_status();
    T();
    E1();
}

void E1(void) {
    if (sym.type == T_ADD || sym.type == T_MINUS) {
        write_detail("使用的产生式：E’ → ATE’ \n");
        print_current_status();
        A();
        T();
        E1();
    } else {
        write_detail("使用的产生式：E’ → 空 \n");
        print_current_status();
    }
}

void T(void) {
    write_detail("使用的产生式：T → FT’ \n");
    print_current_status();
    F();
    T1();
}

void T1(void) {
    if (sym.type == T_MUL || sym.type == T_DIV) {
        write_detail("使用的产生式：T’ → MFT’ \n");
        print_current_status();
        M();
        F();
        T1();
    } else {
        write_detail("使用的产生式：T’ → 空 \n");
        print_current_status();
    }
}

void F(void) {
    if (sym.type == T_LEFT_BRACKET) {
        write_detail("使用的产生式：F → (E) \n");
        print_current_status();
        advance();
        E();
        if (sym.type == T_RIGHT_BRACKET) {
            write_detail("使用的产生式：F → (E) \n");
            print_current_status();
            advance();
        } else {
            write_syntax_error();
        }
    } else {
        write_detail("使用的产生式：F → i \n");
        print_current_status();
        if (sym.type == T_SYMBOL || sym.type == T_CONST) {
            advance();
        } else {
            write_syntax_error();
        }
    }
}

void A(void) {
    write_detail("使用的产生式：A → +|- \n");
    print_current_status();
    if (sym.type == T_ADD || sym.type == T_MINUS) {
        advance();
    } else {
        write_syntax_error();
    }
}

void M(void) {
    write_detail("使用的产生式：M → *|/ \n");
    print_current_status();
    if (sym.type == T_MUL || sym.type == T_DIV) {
        advance();
    } else {
        write_syntax_error();
    }
}


FILE *error_file;
FILE *detail_file;
FILE *symtab1e_file;

void write_error(const char *s) {
    fprintf(stdout, "%s", s);
    fprintf(error_file, "%s", s);
}

void write_detail(const char *s) {
    fprintf(stdout, "%s", s);
    fprintf(detail_file, "%s", s);
}

void write_sym(const char *s) {
    fprintf(symtab1e_file, "%s", s);
}

bool valid_token(char ch) {
    if (isalnum(ch)) {
        return true;
    }
    return ch == '_';
}

void next_token(void) {
    char ch, tmp[MAX_LINE], token_string[MAX_LINE];
    int token;
    if((ch = *(source++)) != '\0') {
        if (isalpha(ch)) {
            // Symble
            int i = 0;
            token_string[i++] = ch;
            while(valid_token(*source)) {
                token_string[i++] = *source++;
            }
            token_string[i] = '\0';
            sym.type = T_SYMBOL;
            sym.value.s = add_symble(token_string);
            return;
        } else if (isdigit(ch)) {
            // 数字
            int num = ch - '0';
            while(isdigit(*source)) {
                num = num * 10 + *source++ - '0';
            }
            sym.type = T_CONST;
            sym.value.v = num;
            return;
        } else {
            switch (ch) {
                case '+':
                    sym.type = T_ADD;
                    sym.value.c = ch;
                    return;
                case '-':
                    sym.type = T_MINUS;
                    sym.value.c = ch;
                    return;
                case '*':
                    sym.type = T_MUL;
                    sym.value.c = ch;
                    return;
                case '/':
                    sym.type = T_DIV;
                    sym.value.c = ch;
                    return;
                case '(':
                    sym.type = T_LEFT_BRACKET;
                    sym.value.c = ch;
                    return;
                case ')':
                    sym.type = T_RIGHT_BRACKET;
                    sym.value.c = ch;
                    return;
            }
            sym.type = T_END;
            return;
        }
    } else {
        sym.type = T_END;
    }
}


void read_source(void) {
    FILE *f = fopen("source.txt", "r");
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    int source_size = ftell(f);
    rewind(f);

    source = (char*) malloc(sizeof(char) * (source_size + 1));
    processed = (char*) malloc(sizeof(char) * (source_size + 1));

    int read_size = fread(source, sizeof(char), source_size, f);

    source[source_size] = '\0';

    fclose(f);

    if (source_size != read_size) {
        // Something went wrong
        exit(EXIT_FAILURE);
    }
}


int main(void){
    detail_file = fopen("detail.txt", "w");
    symtab1e_file = fopen("symtab1.txt", "w");
    error_file = fopen("error.txt", "w");

    if (detail_file == NULL || symtab1e_file == NULL || error_file == NULL) {
        exit(EXIT_FAILURE);
    }

    read_source();

    advance();
    E();

    // Write symbles
    char tmp[MAX_LINE];
    write_sym("符号表\n");
    write_sym("编号\t名称\n");
    for(int i=0; i < MAX_LINE; i++) {
        if(symbles[i] == NULL) {
            break;
        }
        sprintf(tmp, "%d\t%s\n", i+1, symbles[i]->name);
        write_sym(tmp);
    }

    return 0;
}
