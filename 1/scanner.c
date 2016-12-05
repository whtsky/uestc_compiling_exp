#include <stdio.h>


enum Token {
    T_END=-1,
    T_ERROR=0,
    T_SYMBOL=1,
    T_CONSTANT, T_INTEGER, T_IF, T_THEN, T_ELSE,
    T_BEGIN, T_END, T_READ, T_WRITE, T_FUNCTION,
    T_ASSIGN, T_L, T_LE, T_G, T_GE,
    T_ADD, T_SUB, T_MUL, T_DIV,
    T_E, T_NE
};

struct symTableItem{
    char   name[MAX_ID_LENGTH];
    Token  lexToken;
    int    type;
    bool  init;
}

char *reserved_table[] = {
    "integer", "if", "then", "else",
    "begin", "end", "read", "write", "function"
};

int ScanOneToken(FILE *fp){
    int ch = getc(fp); 
        switch(ch) {
        case 'a': case 'b': case 'c': 
            ;
        case '0': case '1': case '2': case '3': 									//.... and other digits
        case ‘/‘:		//除号？注释？
        //case ...:
        case EOF:
            return T_END;
        default:	//错误处理
            return T_ERROR;
    }
}


int main(void){
    FILE* f = fopen("source.txt", "r");
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }
    
    int lineno = 0;
    char *line = NULL;
    size_t len = 0;
    while()
    
    while (ScanOneToken(f) != T_END)
        ; // 处理各个单词符号
    return 0;
}
