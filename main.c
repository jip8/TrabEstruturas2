/*
  Dev by Jip
*/
  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Estrutura nó
typedef struct No {
    char dado;
    struct No *esquerda;
    struct No *direita;
} No;
// Função para criar um novo nó
No* criaNo(char dado) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->dado = dado;
    novoNo->esquerda = novoNo->direita = NULL;
    return novoNo;
}
// verifica se é um operador
int ehOp(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}
// verifica qual a precedencia do operador, 1 é + - e 2 é * /
int obterPrec(char operador) {
    if (operador == '*' || operador == '/')
        return 2;
    else if (operador == '+' || operador == '-')
        return 1;
    else
        return 0;
}
// Função para criar uma árvore
No* constArv(char* expressao, int inicio, int fim) {
    if (inicio > fim) return NULL;

    int parenteses = 0;
    int indiceOperador = -1;
    int precMinima = 10;
    // esse for vare a string e verifica qual operador de menor precedencia e guarda na variavel indiceOperador
    for (int i = inicio; i <= fim; i++) {
      //ignora a parte de dentro do parenteses  
      if (expressao[i] == '(') parenteses++;
        else if (expressao[i] == ')') parenteses--;
        else if (parenteses == 0 && ehOp(expressao[i])) {
            int precAtual = obterPrec(expressao[i]);
            if (precAtual <= precMinima) {
                precMinima = precAtual;
                indiceOperador = i;
            }
        }
    }
    // se indiceOperador nao estiver vazia ele cria um no com base no operador
    if (indiceOperador != -1) {
        No *raiz = criaNo(expressao[indiceOperador]);
        raiz->esquerda = constArv(expressao, inicio, indiceOperador - 1);
        raiz->direita = constArv(expressao, indiceOperador + 1, fim);
        return raiz;
    } 
    // se nao, ele verifica se a funcao inteira esta dentro de parenteses e repassa
    else {
        if (expressao[inicio] == '(' && expressao[fim] == ')') {
            return constArv(expressao, inicio + 1, fim - 1);
        }
        // se nao, ele cria um no unico com o dado da string, pois o que restou é um numero sozinho
        char num[20];
        strncpy(num, expressao + inicio, fim - inicio + 1);
        num[fim - inicio + 1] = '\0';
        return criaNo(num[0]);
    }
}

int calcExp(No* raiz) {
    if (!raiz) return 0;
    if (!raiz->esquerda && !raiz->direita) return raiz->dado - '0';

    int valorEsquerda = calcExp(raiz->esquerda);
    int valorDireita = calcExp(raiz->direita);

    switch (raiz->dado) {
        case '+': return valorEsquerda + valorDireita;
        case '-': return valorEsquerda - valorDireita;
        case '*': return valorEsquerda * valorDireita;
        case '/': return valorEsquerda / valorDireita;
        default: return 0;
    }
}

void imprimirExp(No* raiz) {
    if (!raiz) return;
    if (!raiz->esquerda && !raiz->direita) {
        printf("%c", raiz->dado);
        return;
    }
  
    printf("(");
  
    imprimirExp(raiz->esquerda);

    printf(" %c ", raiz->dado);

    imprimirExp(raiz->direita);

    printf(")");
    
}

void mostraArv(No* raiz, int esp) {
    if (!raiz) return;

    esp += 5;

    mostraArv(raiz->direita, esp);

    printf("\n");
    for (int i = 5; i < esp; i++)
        printf(" ");

    printf("%c\n", raiz->dado);

    mostraArv(raiz->esquerda, esp);
}

void geraStr(No* raiz, int precPar, char* expressao) {
    if (!raiz) return;
    // verifica se o operador tem precedencia menor que a raiz
    precPar = (obterPrec(raiz->dado) < precPar);

    if (raiz->esquerda != NULL || raiz->direita != NULL) {
        if (precPar)
            strcat(expressao, "(");

        geraStr(raiz->esquerda, obterPrec(raiz->dado), expressao);
    }


  sprintf(expressao + strlen(expressao), "%c", raiz->dado);

    if (raiz->esquerda != NULL || raiz->direita != NULL) {
        geraStr(raiz->direita, obterPrec(raiz->dado), expressao);

        if (precPar)
            strcat(expressao, ")");
    }
}

void liberarArv(No* raiz) {
    if (raiz) {
        liberarArv(raiz->esquerda);
        liberarArv(raiz->direita);
        free(raiz);
    }
}

int main() {
    printf("PROJETO DE ESTRUTURAS DE DADOS\n");

    char expressao[100], expInf[100] = "";
    printf("Digite a expressao aritmetica: ");
    fgets(expressao, sizeof(expressao), stdin);

    int len = strlen(expressao);
    expressao[len - 1] = '\0';

    No *raiz = constArv(expressao, 0, strlen(expressao) - 1);

    printf("Formato da arvore:\n");
    mostraArv(raiz, 5);
    printf("\n");

    printf("\n");
    printf("Arvore construIda a partir da expressao:\n");
    imprimirExp(raiz);
    printf("\n");

    printf("\n");
    printf("String em notacao infixada:\n");
    geraStr(raiz,0, expInf);
    printf("%s\n", expInf);

    printf("\n");
    int resultado = calcExp(raiz);
    printf("Resultado da expressao: %d\n", resultado);

    printf("\n");

    liberarArv(raiz);
    
    return 0;
}
