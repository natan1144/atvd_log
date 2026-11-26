#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define QTD 10

typedef struct {
    int matricula;
    char nome[50];
    float nota1, nota2, media;
} Aluno;

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

float lerNota(const char *msg) {
    char linha[64];
    float v;
    while (1) {
        printf("%s", msg);
        if (!fgets(linha, sizeof linha, stdin)) exit(0);
        if (sscanf(linha, "%f", &v) == 1 && v >= 0 && v <= 10) return v;
        printf("Nota invalida (0 a 10).\n");
    }
}

float calcularMedia(float n1, float n2) {
    return (n1 + n2) / 2.0f;
}

void cadastrar(Aluno alunos[]) {
    for (int i = 0; i < QTD; i++) {
        printf("\n--- Aluno %d ---\n", i + 1);
        alunos[i].matricula = lerInt("Matricula: ");
        lerTexto("Nome: ", alunos[i].nome, sizeof alunos[i].nome);
        alunos[i].nota1 = lerNota("Nota 1: ");
        alunos[i].nota2 = lerNota("Nota 2: ");
        alunos[i].media = calcularMedia(alunos[i].nota1, alunos[i].nota2);
    }
}

void exibirSituacao(Aluno alunos[]) {
    printf("\n=== SITUACAO DOS ALUNOS ===\n");
    for (int i = 0; i < QTD; i++) {
        printf("%d - %-20s Media: %.2f - %s\n", alunos[i].matricula, alunos[i].nome,
               alunos[i].media, alunos[i].media >= 6.0f ? "APROVADO" : "REPROVADO");
    }
}

int main(void) {
    Aluno alunos[QTD];
    cadastrar(alunos);
    exibirSituacao(alunos);

    int iMaior = 0, iMenor = 0;
    float soma = 0;
    for (int i = 0; i < QTD; i++) {
        if (alunos[i].media > alunos[iMaior].media) iMaior = i;
        if (alunos[i].media < alunos[iMenor].media) iMenor = i;
        soma += alunos[i].media;
    }
    float mediaGeral = soma / QTD;

    int acima = 0;
    for (int i = 0; i < QTD; i++)
        if (alunos[i].media > mediaGeral) acima++;

    printf("\nMaior media: %s (%.2f)\n", alunos[iMaior].nome, alunos[iMaior].media);
    printf("Menor media: %s (%.2f)\n", alunos[iMenor].nome, alunos[iMenor].media);
    printf("Media geral da turma: %.2f\n", mediaGeral);
    printf("Alunos acima da media geral: %d\n", acima);
    return 0;
}
