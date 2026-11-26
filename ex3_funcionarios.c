#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define QTD 8
#define HORAS_MES 160.0f
#define ACRESCIMO_HE 0.5f
#define DESCONTO 0.10f

typedef struct {
    int codigo;
    char nome[50];
    char cargo[30];
    float salarioBase;
    float horasExtras;
    float salarioBruto;
    float salarioLiquido;
} Funcionario;

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

float calcularValorHora(float salarioBase) {
    return salarioBase / HORAS_MES;
}

float calcularValorHoraExtra(float salarioBase) {
    return calcularValorHora(salarioBase) * (1 + ACRESCIMO_HE);
}

float calcularBruto(Funcionario f) {
    return f.salarioBase + f.horasExtras * calcularValorHoraExtra(f.salarioBase);
}

float calcularLiquido(float bruto) {
    return bruto - bruto * DESCONTO;
}

int main(void) {
    Funcionario func[QTD];

    for (int i = 0; i < QTD; i++) {
        printf("\n--- Funcionario %d ---\n", i + 1);
        func[i].codigo = lerInt("Codigo: ");
        lerTexto("Nome: ", func[i].nome, sizeof func[i].nome);
        lerTexto("Cargo: ", func[i].cargo, sizeof func[i].cargo);
        func[i].salarioBase = lerFloat("Salario-base: R$ ");
        func[i].horasExtras = lerFloat("Horas extras: ");
        func[i].salarioBruto = calcularBruto(func[i]);
        func[i].salarioLiquido = calcularLiquido(func[i].salarioBruto);
    }

    printf("\n=== FOLHA DE PAGAMENTO ===\n");
    float custoTotal = 0;
    int iMaior = 0;
    for (int i = 0; i < QTD; i++) {
        printf("%d - %-15s (%s)\n", func[i].codigo, func[i].nome, func[i].cargo);
        printf("   Valor da hora: R$ %.2f | Hora extra: R$ %.2f\n",
               calcularValorHora(func[i].salarioBase), calcularValorHoraExtra(func[i].salarioBase));
        printf("   Bruto: R$ %.2f | Liquido: R$ %.2f\n", func[i].salarioBruto, func[i].salarioLiquido);
        custoTotal += func[i].salarioLiquido;
        if (func[i].salarioLiquido > func[iMaior].salarioLiquido) iMaior = i;
    }

    printf("\nMaior salario liquido: %s (R$ %.2f)\n", func[iMaior].nome, func[iMaior].salarioLiquido);
    printf("Custo total da folha (liquido): R$ %.2f\n", custoTotal);
    return 0;
}
