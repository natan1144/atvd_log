#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PAC 15
#define MAX_CONS 30

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
} Paciente;

typedef struct {
    Paciente paciente;
    Data data;
    char descricao[100];
} Consulta;

Paciente pacientes[MAX_PAC];
Consulta consultas[MAX_CONS];
int totalPac = 0, totalCons = 0;

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

Data lerData(void) {
    Data d;
    while (1) {
        d.dia = lerInt("Dia: ");
        d.mes = lerInt("Mes: ");
        d.ano = lerInt("Ano: ");
        if (d.dia >= 1 && d.dia <= 31 && d.mes >= 1 && d.mes <= 12 && d.ano >= 1900) return d;
        printf("Data invalida.\n");
    }
}

int mesmaData(Data a, Data b) {
    return a.dia == b.dia && a.mes == b.mes && a.ano == b.ano;
}

int buscarPaciente(int codigo) {
    for (int i = 0; i < totalPac; i++)
        if (pacientes[i].codigo == codigo) return i;
    return -1;
}

void cadastrarPaciente(void) {
    if (totalPac >= MAX_PAC) {
        printf("Limite de pacientes atingido.\n");
        return;
    }
    int codigo = lerInt("Codigo do paciente: ");
    if (buscarPaciente(codigo) != -1) {
        printf("Codigo ja cadastrado!\n");
        return;
    }
    pacientes[totalPac].codigo = codigo;
    lerTexto("Nome: ", pacientes[totalPac].nome, sizeof pacientes[totalPac].nome);
    totalPac++;
    printf("Paciente cadastrado.\n");
}

void exibirConsulta(const Consulta *c) {
    printf("%02d/%02d/%04d | %s (cod %d) | %s\n", c->data.dia, c->data.mes, c->data.ano,
           c->paciente.nome, c->paciente.codigo, c->descricao);
}

void registrarConsulta(void) {
    if (totalCons >= MAX_CONS) {
        printf("Limite de consultas atingido.\n");
        return;
    }
    int i = buscarPaciente(lerInt("Codigo do paciente: "));
    if (i == -1) {
        printf("Paciente nao encontrado.\n");
        return;
    }
    Consulta *c = &consultas[totalCons];
    c->paciente = pacientes[i];
    printf("Data da consulta:\n");
    c->data = lerData();
    lerTexto("Descricao do atendimento: ", c->descricao, sizeof c->descricao);
    totalCons++;
    printf("Consulta registrada.\n");
}

void consultasDoPaciente(void) {
    int codigo = lerInt("Codigo do paciente: ");
    int achou = 0;
    for (int i = 0; i < totalCons; i++)
        if (consultas[i].paciente.codigo == codigo) {
            exibirConsulta(&consultas[i]);
            achou = 1;
        }
    if (!achou) printf("Nenhuma consulta encontrada para esse codigo.\n");
}

void consultasPorData(void) {
    printf("Data a pesquisar:\n");
    Data d = lerData();
    int achou = 0;
    for (int i = 0; i < totalCons; i++)
        if (mesmaData(consultas[i].data, d)) {
            exibirConsulta(&consultas[i]);
            achou = 1;
        }
    if (!achou) printf("Nenhuma consulta nessa data.\n");
}

int contarConsultas(int codigo) {
    int n = 0;
    for (int i = 0; i < totalCons; i++)
        if (consultas[i].paciente.codigo == codigo) n++;
    return n;
}

void contabilizar(void) {
    if (totalPac == 0) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }
    for (int i = 0; i < totalPac; i++)
        printf("%s (cod %d): %d consulta(s)\n", pacientes[i].nome, pacientes[i].codigo,
               contarConsultas(pacientes[i].codigo));
}

void pacienteMaisAtendido(void) {
    if (totalPac == 0 || totalCons == 0) {
        printf("Sem dados suficientes.\n");
        return;
    }
    int iMaior = 0, maior = contarConsultas(pacientes[0].codigo);
    for (int i = 1; i < totalPac; i++) {
        int n = contarConsultas(pacientes[i].codigo);
        if (n > maior) {
            maior = n;
            iMaior = i;
        }
    }
    printf("Paciente com mais atendimentos: %s (%d consultas)\n", pacientes[iMaior].nome, maior);
}

int main(void) {
    int op;
    do {
        printf("\n===== CLINICA =====\n");
        printf("1 - Cadastrar paciente\n2 - Registrar consulta\n3 - Consultas de um paciente\n");
        printf("4 - Consultas por data\n5 - Consultas por paciente (contagem)\n");
        printf("6 - Paciente com mais atendimentos\n0 - Sair\n");
        op = lerInt("Opcao: ");
        switch (op) {
            case 1: cadastrarPaciente(); break;
            case 2: registrarConsulta(); break;
            case 3: consultasDoPaciente(); break;
            case 4: consultasPorData(); break;
            case 5: contabilizar(); break;
            case 6: pacienteMaisAtendido(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
    return 0;
}
