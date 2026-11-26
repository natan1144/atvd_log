#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 15

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto;

Produto produtos[MAX];
int total = 0;

void lerTexto(const char *msg, char *buf, int tam) {
    printf("%s", msg);
    if (!fgets(buf, tam, stdin)) exit(0);
    buf[strcspn(buf, "\n")] = '\0';
}

int lerInt(const char *msg) {
    char linha[64];
    int v;
    while (1) {
        printf("%s", msg);
        if (!fgets(linha, sizeof linha, stdin)) exit(0);
        if (sscanf(linha, "%d", &v) == 1) return v;
        printf("Valor invalido.\n");
    }
}

float lerFloat(const char *msg) {
    char linha[64];
    float v;
    while (1) {
        printf("%s", msg);
        if (!fgets(linha, sizeof linha, stdin)) exit(0);
        if (sscanf(linha, "%f", &v) == 1 && v >= 0) return v;
        printf("Valor invalido.\n");
    }
}

int buscarPorCodigo(int codigo) {
    for (int i = 0; i < total; i++)
        if (produtos[i].codigo == codigo) return i;
    return -1;
}

void cadastrar(void) {
    if (total >= MAX) {
        printf("Limite de %d produtos atingido.\n", MAX);
        return;
    }
    int codigo = lerInt("Codigo: ");
    if (buscarPorCodigo(codigo) != -1) {
        printf("Codigo ja cadastrado!\n");
        return;
    }
    Produto *p = &produtos[total];
    p->codigo = codigo;
    lerTexto("Nome: ", p->nome, sizeof p->nome);
    p->preco = lerFloat("Preco: R$ ");
    p->estoque = lerInt("Quantidade em estoque: ");
    if (p->estoque < 0) p->estoque = 0;
    total++;
    printf("Produto cadastrado.\n");
}

void exibirProduto(const Produto *p) {
    printf("Cod: %d | %-20s | R$ %.2f | Estoque: %d | Total: R$ %.2f\n",
           p->codigo, p->nome, p->preco, p->estoque, p->preco * p->estoque);
}

void listar(void) {
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    float valorTotal = 0;
    int iMaior = 0;
    for (int i = 0; i < total; i++) {
        exibirProduto(&produtos[i]);
        float v = produtos[i].preco * produtos[i].estoque;
        valorTotal += v;
        if (v > produtos[iMaior].preco * produtos[iMaior].estoque) iMaior = i;
    }
    printf("\nValor total em estoque: R$ %.2f\n", valorTotal);
    printf("Produto com maior valor em estoque: %s (R$ %.2f)\n", produtos[iMaior].nome,
           produtos[iMaior].preco * produtos[iMaior].estoque);
}

void reporEstoque(void) {
    int i = buscarPorCodigo(lerInt("Codigo do produto: "));
    if (i == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }
    int qtd = lerInt("Quantidade a repor: ");
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    produtos[i].estoque += qtd;
    printf("Novo estoque: %d\n", produtos[i].estoque);
}

void registrarVenda(void) {
    int i = buscarPorCodigo(lerInt("Codigo do produto: "));
    if (i == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }
    int qtd = lerInt("Quantidade vendida: ");
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    if (qtd > produtos[i].estoque) {
        printf("Venda negada! Estoque disponivel: %d\n", produtos[i].estoque);
        return;
    }
    produtos[i].estoque -= qtd;
    printf("Venda registrada. Total: R$ %.2f | Estoque restante: %d\n",
           qtd * produtos[i].preco, produtos[i].estoque);
}

void consultar(void) {
    int i = buscarPorCodigo(lerInt("Codigo do produto: "));
    if (i == -1) printf("Produto nao encontrado.\n");
    else exibirProduto(&produtos[i]);
}

int main(void) {
    int op;
    do {
        printf("\n===== PAPELARIA =====\n");
        printf("1 - Cadastrar produto\n2 - Listar produtos\n3 - Repor estoque\n");
        printf("4 - Registrar venda\n5 - Consultar por codigo\n0 - Sair\n");
        op = lerInt("Opcao: ");
        switch (op) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: reporEstoque(); break;
            case 4: registrarVenda(); break;
            case 5: consultar(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
    return 0;
}
