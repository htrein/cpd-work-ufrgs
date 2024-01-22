
/* tipos de dados */

typedef struct{
    char nome[100];
    int id_ind;
}INDUSTRIA;

typedef struct{
    char nome[100];
    char website[100];
    int id_emp;
    INDUSTRIA *chave_estrangeira1;
}EMPRESA;

typedef struct{
    char nome[100];
    int id_local;
}LOCALIZACAO;

typedef struct{
    char nome[100];
    char descricao[1000];
    int id_cargo;
    LOCALIZACAO *chave_estrangeira1;
}CARGO;

typedef struct{
    CARGO *chave_estrangeira1;
    EMPRESA *chave_estrangeira2;
    char data_criacao[100];
}OFERTA;

typedef struct{
    char nome[150];
    char tipo[100];
    int id_crit;
}CRITERIOS;

typedef struct{
    CRITERIOS *chave_estrangeira1;
    CARGO *chave_estrangeira2;
}REQUISITA;
