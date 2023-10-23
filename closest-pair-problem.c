#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 10000

typedef struct Ponto{
    int x;
    int y;
}Ponto;

static Ponto pontos[MAX];

void inicializa_pontos(Ponto pontos[], int qtd);

void mostra_pontos(Ponto pontos[], int qtd);

void counting_sort_x(Ponto vet[], int tam);

void counting_sort_y(Ponto vet[], int tam);

double distancia(Ponto p1, Ponto p2);

double mais_prox_recurs(Ponto pontos[], int inicio, int fim);

double par_mais_proximo1(Ponto pontos[], int qtd);

double par_mais_proximo2(Ponto pontos[], int qtd);

int main() {
    clock_t start, end;
    double cpu_time_used;

    FILE *saida = fopen("saida.txt", "w");

    for(int n = 2000; n <= MAX; n += 2000) {

        inicializa_pontos(pontos, n);

        printf("\nQuantidade de pontos: %d\n", n);
        fprintf(saida, "\nQuantidade de pontos: %d\n", n);

        start = clock();
        double resp = par_mais_proximo1(pontos, n);
        end = clock();
        printf("\nAlgoritmo 1:\n Resposta: %.3lf\n", resp);
        printf(" Tempo de execução: %.3f s\n", ((double) (end - start)) / CLOCKS_PER_SEC);
        fprintf(saida, "\nAlgoritmo 1:\n Resposta: %.3lf\n", resp);
        fprintf(saida, " Tempo de execução: %.3f s\n", ((double) (end - start)) / CLOCKS_PER_SEC);

        start = clock();
        resp = par_mais_proximo2(pontos, n);
        end = clock();
        printf("\nAlgoritmo 2:\n Resposta: %.3lf\n", resp);
        printf(" Tempo de execução: %.3f s\n", ((double) (end - start)) / CLOCKS_PER_SEC);
        fprintf(saida, "\nAlgoritmo 2:\n Resposta: %.3lf\n", resp);
        fprintf(saida, " Tempo de execução: %.3f s\n", ((double) (end - start)) / CLOCKS_PER_SEC);

        printf("\n---------------------------------------------------\n");
        fprintf(saida, "\n---------------------------------------------------\n");
    }

    return 0;
}

void inicializa_pontos(Ponto pontos[], int qtd) {
    srand(time(NULL));
    
    for(int i = 0; i < qtd; i++) {
        pontos[i].x = rand() % 300;
        pontos[i].y = rand() % 300;
    }
}

void mostra_pontos(Ponto pontos[], int qtd) {
    for(int i = 0; i < qtd; i++)
        printf("(%d, %d) ", pontos[i].x, pontos[i].y);
}

void counting_sort_x(Ponto vet[], int tam) {
    // achar o maior valor
    Ponto max = vet[0];
    for(int i = 0; i < tam; i++) {
        if (vet[i].x > max.x) 
            max = vet[i];
    }

    // iniciar o vetor de count com 0
    int *count_vet = calloc(max.x + 1, sizeof(int));

    // mapear cada elemento do vetor de entrada para vetor de count
    for (int i = 0; i < tam; i++) 
        count_vet[vet[i].x]++;

    // calcular a soma cumulativa de cada indice
    for (int i = 1; i <= max.x; i++)
        count_vet[i] += count_vet[i-1];

    // criar vetor de resultado
    Ponto *resultado = malloc(tam * sizeof(Ponto));
    for (int i = tam - 1; i >= 0; i--) {
        resultado[count_vet[vet[i].x] - 1] = vet[i];
        count_vet[vet[i].x]--;
    }
    free(count_vet);

    // copiar vetor resultado
    for(int i = 0; i < tam; i++)
        vet[i] = resultado[i];
}

void counting_sort_y(Ponto vet[], int tam) {
    // achar o maior valor
    Ponto max = vet[0];
    for(int i = 0; i < tam; i++) {
        if (vet[i].y > max.y) 
            max = vet[i];
    }

    // iniciar o vetor de count com 0
    int *count_vet = calloc(max.y + 1, sizeof(int));

    // mapear cada elemento do vetor de entrada para vetor de count
    for (int i = 0; i < tam; i++) 
        count_vet[vet[i].y]++;

    // calcular a soma cumulativa de cada indice
    for (int i = 1; i <= max.y; i++)
        count_vet[i] += count_vet[i-1];

    // criar vetor de resultado
    Ponto *resultado = malloc(tam * sizeof(Ponto));
    for (int i = tam - 1; i >= 0; i--) {
        resultado[count_vet[vet[i].y] - 1] = vet[i];
        count_vet[vet[i].y]--;
    }
    free(count_vet);

    // copiar vetor resultado
    for(int i = 0; i < tam; i++)
        vet[i] = resultado[i];
}

double distancia(Ponto p1, Ponto p2) {
    int x_dist = pow((p1.x + p2.x), 2);
    int y_dist = pow((p1.y + p2.y), 2);
    return sqrt((x_dist + y_dist));
}

double mais_prox_recurs(Ponto pontos[], int inicio, int fim) {
    double min = INFINITY;
    for(int i = inicio; i < fim; i++) {
        for(int j = inicio; j < fim; j++) {
            if (i == j)
                continue;
            double d = distancia(pontos[i], pontos[j]);
            if (d < min)
                min = d;
        }
    }
    return min;
}

double par_mais_proximo1(Ponto pontos[], int qtd) {

    // ordenar pontos pela coordenada x
    counting_sort_x(pontos, qtd);

    // dividir em P1 e P2
    int meio = qtd/2;

    // recursivamente calcular as menores distancias d1 e d2
    double d1 = mais_prox_recurs(pontos, 0, meio);
    double d2 = mais_prox_recurs(pontos, meio, qtd);

    // obter menor distancia entre d1 e d2
    double s = d1 < d2 ? d1 : d2;

    // eliminar os pontos que estão a uma distancia maior que s da linha vertical
    Ponto *aux = malloc(qtd * sizeof(Ponto));
    int count = 0;
    for(int i = 0; i < qtd; i++) {
        if (abs(pontos[i].x - pontos[meio].x) < s) {
            aux[count] = pontos[i];
            count++;
        }
    }

    // ordenar pontos pela coordenada y
    counting_sort_y(aux, count);

    // calcular a menor distancia entre os pontos restantes a no maximo 7 vizinhos
    for(int i = 0; i < count; i++){
        for(int j = i+1; j < count && abs(aux[j].y - aux[i].y) < s; j++) {
            double d = distancia(aux[i], aux[j]);
            if (d < s)
                s = d;
        }
    }
    free(aux);

    return s;
}

double par_mais_proximo2(Ponto pontos[], int qtd) {

    // ordenar pontos pela coordenada x
    counting_sort_x(pontos, qtd);

    // dividir em P1 e P2
    int meio = qtd/2;
    Ponto *p1 = malloc(meio * sizeof(Ponto));
    Ponto *p2 = malloc(meio * sizeof(Ponto));
    for(int i = 0; i < meio; i++) {
        p1[i] = pontos[i];
        p2[i] = pontos[i + meio];
    }

    // recursivamente calcular as menores distancias d1 e d2
    double d1 = mais_prox_recurs(p1, 0, meio);
    double d2 = mais_prox_recurs(p2, 0, meio);

    // ordenar p1 e p2 pela coordenada y
    counting_sort_y(p1, meio);
    counting_sort_y(p2, meio);

    // combinar p1 e p2 em um único vetor ordenado por y
    Ponto *aux = malloc(qtd * sizeof(Ponto));
    int i = 0, j = 0, k = 0;
    while(i < meio && j < meio) {
        if (p1[i].y < p2[j].y) {
            aux[k] = p1[i];
            i++;
        } else {
            aux[k] = p2[j];
            j++;
        }
        k++;
    }
    while(i < meio) {
        aux[k] = p1[i];
        i++;
        k++;
    }
    while(j < meio) {
        aux[k] = p2[j];
        j++;
        k++;
    }
    free(p1);
    free(p2);

    // obter menor distancia entre d1 e d2
    double s = d1 < d2 ? d1 : d2;

    // eliminar os pontos que estão a uma distancia maior que s da linha vertical
    Ponto *aux2 = malloc(qtd * sizeof(Ponto));
    int count = 0;
    for(int i = 0; i < qtd; i++) {
        if (abs(aux[i].x - aux[meio].x) < s) {
            aux2[count] = aux[i];
            count++;
        }
    }
    free(aux);

    // calcular a menor distancia entre os pontos restantes a no maximo 7 vizinhos
    for(int i = 0; i < count; i++){
        for(int j = i+1; j < count && abs(aux2[j].y - aux2[i].y) < s; j++) {
            double d = distancia(aux2[i], aux2[j]);
            if (d < s)
                s = d;
        }
    }
    free(aux2);

    return s;
}

