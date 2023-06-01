#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hospede {
    int quarto;
    char nome[80];
    int acompanhante;
    char categoria;
    int dias;
} hospede;

typedef struct quarto {
    int num;
    char categoria;
    char status;
} quarto;

void aloca_hospede(hospede **ph, int tam);
void aloca_quarto(quarto **pq, int tam);
void cadastro_hospede(hospede *ph, quarto *pq, int tam);
int verifica_hospede();
void grava_hospede(hospede *ph);
int busca_quarto(quarto *pq, int tam, char categoria);
void cadastro_quarto(quarto *pq, int total_quartos);
void checkout(hospede *ph, quarto *pq, int tam, int quarto);
void mostrar_hospedes();
void remover_hospede(int index);

int main() {
    hospede *ph = NULL;
    quarto *pq = NULL;
    int op, cont = 0;
    aloca_quarto(&pq, 15);
    cadastro_quarto(pq, 15);

    do {
        printf("[1] Cadastro\n");
        printf("[2] Check-out\n");
        printf("[3] Mostrar Hóspedes\n");
        printf("[4] Sair\n");
        scanf("%i", &op);
        fflush(stdin);

        switch (op) {
            case 1:
                aloca_hospede(&ph, cont + 1);
                cadastro_hospede(&ph[cont], pq, 15);
                cont++;
                break;
            case 2:
                printf("Digite o número do quarto para check-out: ");
                int num_quarto;
                scanf("%d", &num_quarto);
                checkout(ph, pq, 15, num_quarto);
                break;
            case 3:
                mostrar_hospedes();
                break;
        }
    } while (op != 4);

    free(ph);
    free(pq);
    return 0;
}

void aloca_hospede(hospede **ph, int tam) {
    if ((*ph = (hospede*)realloc(*ph, tam * sizeof(hospede))) == NULL) {
        exit(1);
    }
}

void aloca_quarto(quarto **pq, int tam) {
    if ((*pq = (quarto*)realloc(*pq, tam * sizeof(quarto))) == NULL) {
        exit(1);
    }
}

void cadastro_hospede(hospede *ph, quarto *pq, int tam) {
    int qtd, diaria_solteiro = 160, diaria_casal = 260;
    float valor_total;

    qtd = verifica_hospede();
    ph->quarto = qtd + 1;

    printf("\ndigite o seu nome: ");
    fflush(stdin);
    fgets(ph->nome, sizeof(ph->nome), stdin);
    ph->nome[strcspn(ph->nome, "\n")] = '\0';

    printf("\ndigite a quantidade de dias: ");
    scanf("%i", &(ph->dias));
    fflush(stdin);

    printf("\ndigite a quantidade de acompanhantes [max 3]: ");
    scanf("%i", &(ph->acompanhante));
    fflush(stdin);

    if (ph->acompanhante >= 1 && ph->acompanhante <= 3) {
        ph->categoria = 'F';
        valor_total = (ph->dias) * diaria_casal;
    } else if (ph->acompanhante == 0) {
        ph->categoria = 'S';
        valor_total = (ph->dias) * diaria_solteiro;
    } else {
        printf("Quantidade de acompanhantes inválida!\n");
        return;
    }

    printf("Valor total: %.2f\n", valor_total);

    ph->quarto = busca_quarto(pq, tam, ph->categoria);
    if (ph->quarto == -1) {
        printf("Quarto indisponível\n");
    } else {
        printf("Cadastro realizado com sucesso - Quarto: %i\n\n", ph->quarto);
    }
    system("pause");
    grava_hospede(ph);
}

int verifica_hospede() {
    FILE *fh = NULL;
    long int cont = 0;
    if ((fh = fopen("hospede.bin", "rb")) == NULL) {
        return cont;
    } else {
        fseek(fh, 0, SEEK_END);
        cont = ftell(fh) / sizeof(hospede);
        fclose(fh);
        return cont;
    }
}

void grava_hospede(hospede *ph) {
    FILE *fh = NULL;
    if ((fh = fopen("hospede.bin", "ab")) == NULL) {
        printf("Erro ao abrir o arquivo, não existe\n");
        exit(1);
    } else {
        fwrite(ph, sizeof(hospede), 1, fh);
    }
    fclose(fh);
}

int busca_quarto(quarto *pq, int tam, char categoria) {
    int i;
    for (i = 0; i < tam; i++, pq++) {
        if (pq->categoria == categoria && pq->status == 'L') {
            pq->status = 'O';
            return pq->num;
        }
    }
    return -1;
}

void cadastro_quarto(quarto *pq, int total_quartos) {
    int i;
    char categoria;

    printf("Cadastro de Quartos\n");
    for (i = 0; i < total_quartos; i++) {
        printf("Quarto %d\n", i + 1);

        if (i < 5) {
            pq[i].categoria = 'S';  // Quarto de solteiro
        } else {
            pq[i].categoria = 'F';  // Quarto de família
        }

        pq[i].num = i + 1;
        pq[i].status = 'L';  // Quarto livre

        printf("Categoria: %c\n", pq[i].categoria);
        printf("Número: %d\n", pq[i].num);
        printf("Status: %c\n", pq[i].status);
        printf("\n");
    }

    FILE *fq;
    fq = fopen("quartos.bin", "wb");
    if (fq == NULL) {
        printf("Erro ao abrir o arquivo 'quartos.bin'\n");
        exit(1);
    }
    fwrite(pq, sizeof(quarto), total_quartos, fq);
    fclose(fq);
}

void checkout(hospede *ph, quarto *pq, int tam, int quarto) {
    int i;

    for (i = 0; i < tam; i++) {
        if (ph[i].quarto == quarto) {
            printf("\nNome: %s\n", ph[i].nome);
            printf("Quarto: %d\n", ph[i].quarto);
            printf("Dias: %d\n", ph[i].dias);
            printf("Acompanhantes: %d\n", ph[i].acompanhante);

            float valor_total;
            int diaria_solteiro = 160, diaria_casal = 260;

            if (ph[i].acompanhante >= 1 && ph[i].acompanhante <= 3) {
                valor_total = (ph[i].dias) * diaria_casal;
            } else if (ph[i].acompanhante == 0) {
                valor_total = (ph[i].dias) * diaria_solteiro;
            }

            printf("Valor total: %.2f\n\n", valor_total);

            pq[quarto - 1].status = 'L';  // Marcar quarto como livre

            // Remover hóspede do arquivo
            FILE *fh = NULL;
            FILE *temp = NULL;
            int index = i;
            int count = 0;

            fh = fopen("hospede.bin", "rb");
            if (fh == NULL) {
                printf("Erro ao abrir o arquivo 'hospede.bin'\n");
                exit(1);
            }

            temp = fopen("temp.bin", "wb");
            if (temp == NULL) {
                printf("Erro ao abrir o arquivo 'temp.bin'\n");
                exit(1);
            }

            hospede temp_hospede;

            while (fread(&temp_hospede, sizeof(hospede), 1, fh)) {
                if (count != index) {
                    fwrite(&temp_hospede, sizeof(hospede), 1, temp);
                }
                count++;
            }

            fclose(fh);
            fclose(temp);

            remove("hospede.bin");
            rename("temp.bin", "hospede.bin");

            break;
        }
    }

    if (i == tam) {
        printf("Quarto não encontrado!\n\n");
    }
}

void mostrar_hospedes() {
    FILE *fh;
    hospede temp_hospede;

    fh = fopen("hospede.bin", "rb");
    if (fh == NULL) {
        printf("Erro ao abrir o arquivo 'hospede.bin'\n");
        exit(1);
    }

    printf("\nHóspedes:\n");
    while (fread(&temp_hospede, sizeof(hospede), 1, fh)) {
        printf("Nome: %s\n", temp_hospede.nome);
        printf("Quarto: %d\n", temp_hospede.quarto);
        printf("Dias: %d\n", temp_hospede.dias);
        printf("Acompanhantes: %d\n", temp_hospede.acompanhante);

        float valor_total;
        int diaria_solteiro = 160, diaria_casal = 260;

        if (temp_hospede.acompanhante >= 1 && temp_hospede.acompanhante <= 3) {
            valor_total = (temp_hospede.dias) * diaria_casal;
        } else if (temp_hospede.acompanhante == 0) {
            valor_total = (temp_hospede.dias) * diaria_solteiro;
        }

        printf("Valor total: %.2f\n\n", valor_total);
    }

    fclose(fh);
}

void remover_hospede(int index) {
    FILE *fh = NULL;
    FILE *temp = NULL;
    int count = 0;

    fh = fopen("hospede.bin", "rb");
    if (fh == NULL) {
        printf("Erro ao abrir o arquivo 'hospede.bin'\n");
        exit(1);
    }

    temp = fopen("temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo 'temp.bin'\n");
        exit(1);
    }

    hospede temp_hospede;

    while (fread(&temp_hospede, sizeof(hospede), 1, fh)) {
        if (count != index) {
            fwrite(&temp_hospede, sizeof(hospede), 1, temp);
        }
        count++;
    }

    fclose(fh);
    fclose(temp);

    remove("hospede.bin");
    rename("temp.bin", "hospede.bin");
}
