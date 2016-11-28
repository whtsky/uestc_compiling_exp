#include <stdbool.h>

enum Token {
    T_SYMBOL=1,
    T_CONSTANT, T_INT, T_IF
};

struct symTableItem{
    char   name[MAX_ID_LENGTH];
    Token  lexToken;
    int    type;
    bool  init;
}

char *reserved_table[] = {
    "int", "if", "else",
    "while", "for", "read", "write"
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
    struct token_t token;
    InitScanner();
    FILE* f = fopen(argv[1], "r");   
    while (ScanOneToken(f) != T_END)
        ; // 处理各个单词符号
    return 0;
}
