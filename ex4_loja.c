#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROD 10
#define MAX_VENDAS 20

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto;

typedef struct {
    Produto produto;   /* dados do produto no momento da venda */
    int quantidade;
    float valorTotal;
} Venda;

Produto produtos[MAX_PROD];
Venda vendas[MAX_VENDAS];
int totalProd = 0, totalVendas = 0;

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

int buscarProduto(int codigo) {
    for (int i = 0; i < totalProd; i++)
        if (produtos[i].codigo == codigo) return i;
    return -1;
}

void cadastrarProduto(void) {
    if (totalProd >= MAX_PROD) {
        printf("Limite de produtos atingido.\n");
        return;
    }
    int codigo = lerInt("Codigo: ");
    if (buscarProduto(codigo) != -1) {
        printf("Codigo ja cadastrado!\n");
        return;
    }
    Produto *p = &produtos[totalProd];
    p->codigo = codigo;
    lerTexto("Nome: ", p->nome, sizeof p->nome);
    p->preco = lerFloat("Preco: R$ ");
    p->estoque = lerInt("Estoque: ");
    if (p->estoque < 0) p->estoque = 0;
    totalProd++;
    printf("Produto cadastrado.\n");
}

void listarProdutos(void) {
    if (totalProd == 0) printf("Nenhum produto cadastrado.\n");
    for (int i = 0; i < totalProd; i++)
        printf("Cod: %d | %-20s | R$ %.2f | Estoque: %d\n",
               produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].estoque);
}

void registrarVenda(void) {
    if (totalVendas >= MAX_VENDAS) {
        printf("Limite de vendas atingido.\n");
        return;
    }
    int i = buscarProduto(lerInt("Codigo do produto vendido: "));
    if (i == -1) {
        printf("Produto nao cadastrado.\n");
        return;
    }
    int qtd = lerInt("Quantidade vendida: ");
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    if (qtd > produtos[i].estoque) {
        printf("Estoque insuficiente (disponivel: %d).\n", produtos[i].estoque);
        return;
    }
    produtos[i].estoque -= qtd;                       /* atualiza estoque */
    vendas[totalVendas].produto = produtos[i];
    vendas[totalVendas].quantidade = qtd;
    vendas[totalVendas].valorTotal = qtd * produtos[i].preco;
    printf("Venda registrada: R$ %.2f\n", vendas[totalVendas].valorTotal);
    totalVendas++;
}

void listarVendas(void) {
    if (totalVendas == 0) printf("Nenhuma venda registrada.\n");
    for (int i = 0; i < totalVendas; i++)
        printf("Venda %d | %-20s | Qtd: %d | Total: R$ %.2f\n", i + 1,
               vendas[i].produto.nome, vendas[i].quantidade, vendas[i].valorTotal);
}

void relatorio(void) {
    if (totalVendas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    float faturamento = 0;
    for (int i = 0; i < totalVendas; i++) faturamento += vendas[i].valorTotal;
    printf("Faturamento total: R$ %.2f\n", faturamento);

    float maior = -1;
    int iMaior = -1;
    for (int p = 0; p < totalProd; p++) {
        float soma = 0;
        for (int v = 0; v < totalVendas; v++)
            if (vendas[v].produto.codigo == produtos[p].codigo) soma += vendas[v].valorTotal;
        if (soma > maior) {
            maior = soma;
            iMaior = p;
        }
    }
    printf("Produto de maior faturamento: %s (R$ %.2f)\n", produtos[iMaior].nome, maior);
}

int main(void) {
    int op;
    do {
        printf("\n===== LOJA =====\n");
        printf("1 - Cadastrar produto\n2 - Listar produtos\n3 - Registrar venda\n");
        printf("4 - Listar vendas\n5 - Relatorio (faturamento)\n0 - Sair\n");
        op = lerInt("Opcao: ");
        switch (op) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: registrarVenda(); break;
            case 4: listarVendas(); break;
            case 5: relatorio(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
    return 0;
}
