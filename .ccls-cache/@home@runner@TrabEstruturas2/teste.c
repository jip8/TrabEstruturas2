#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Definição da estrutura da árvore binária
typedef struct No {
    char dado;
    struct No *esquerda;
    struct No *direita;
} No;

// Função para criar um novo nó da árvore
No* criarNo(char dado) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->dado = dado;
    novoNo->esquerda = novoNo->direita = NULL;
    return novoNo;
}

// Função para verificar se o caractere é um operador
bool ehOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Função para construir a árvore a partir da expressão
No* construirArvoreExpressao(char* expressao, int inicio, int fim) {
    if (inicio > fim) return NULL;

    No *raiz = NULL;
    int parenteses = 0;

    // Encontrar o operador de menor prioridade dentro dos parênteses mais externos
    for (int i = fim; i >= inicio; i--) {
        if (expressao[i] == '(') parenteses++;
        else if (expressao[i] == ')') parenteses--;
        else if (parenteses == 0 && ehOperador(expressao[i])) {
            raiz = criarNo(expressao[i]);
            raiz->esquerda = construirArvoreExpressao(expressao, inicio, i - 1);
            raiz->direita = construirArvoreExpressao(expressao, i + 1, fim);
            break;
        }
    }

    // Se não encontrou um operador, considera que é um número
    if (!raiz) {
        if (expressao[inicio] == '(' && expressao[fim] == ')') {
            return construirArvoreExpressao(expressao, inicio + 1, fim - 1);
        }
        // Construir nó com número
        char num[2];
        num[0] = expressao[inicio];
        num[1] = '\0';
        raiz = criarNo(num[0]);
    }

    return raiz;
}

// Função para calcular o resultado da expressão
int calcularExpressao(No* raiz) {
    if (!raiz) return 0;
    if (!raiz->esquerda && !raiz->direita) return raiz->dado - '0';

    int valorEsquerda = calcularExpressao(raiz->esquerda);
    int valorDireita = calcularExpressao(raiz->direita);

    switch (raiz->dado) {
        case '+': return valorEsquerda + valorDireita;
        case '-': return valorEsquerda - valorDireita;
        case '*': return valorEsquerda * valorDireita;
        case '/': return valorEsquerda / valorDireita;
        default: return 0;
    }
}

// Função para imprimir a expressão infixada
void imprimirExpressaoInfixada(No* raiz) {
    if (!raiz) return;
    if (!raiz->esquerda && !raiz->direita) {
        printf("%c", raiz->dado);
        return;
    }
    printf("(");
    imprimirExpressaoInfixada(raiz->esquerda);
    printf(" %c ", raiz->dado);
    imprimirExpressaoInfixada(raiz->direita);
    printf(")");
}

// Função para liberar a memória ocupada pela árvore
void liberarArvore(No* raiz) {
    if (raiz) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    char expressao[] = "(5+3)*4/2"; // Sua expressão aritmética

    No *raiz = construirArvoreExpressao(expressao, 0, strlen(expressao) - 1);

    printf("Árvore construída a partir da expressão:\n");
    imprimirExpressaoInfixada(raiz);
    printf("\n");

    int resultado = calcularExpressao(raiz);
    printf("Resultado da expressão: %d\n", resultado);

    printf("Expressão infixada a partir da árvore:\n");
    imprimirExpressaoInfixada(raiz);
    printf("\n");

    // Liberar memória da árvore
    liberarArvore(raiz);

    return 0;
}
