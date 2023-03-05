#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "serial-basic.h"
#include "image-lib.h"

/////////////////// variaveis globais ///////////////////

char **lista_i; // tabela de imagens a processar

/////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

    //////////////////// variaveis ////////////////////////

    int i = 0, N = 0, div = 0;
    char diretoria[100];   // diretoria das imagens
    char list[100];        // path para o ficheiro image-list.txt
    char linha[100] = " "; // buffer de leitura
    FILE *Imagens;
    int N_imagens = 0;
    int Num_threads; // numero de threads a criar
    int *Imagens_pt; // numero de imagens por thread
    struct Argumentos **argumentos;
    pthread_t *thread_id;
    void *thread_ret;
    int N_cores;

    //////////////////////////////////////////////////////

    strcpy(diretoria, argv[1]);
    strcpy(list, argv[1]);
    strcat(list, "/image-list.txt");

    // Convercao do argumento para int (assumimos que argv[2] < 100)

    if (strlen(argv[2]) == 2)
    {
        Num_threads = ((argv[2][0]) - 48) * 10 + ((argv[2][1]) - 48);
    }
    else
    {
        Num_threads = (argv[2][0]) - 48;
    }
    N_cores = sysconf(_SC_NPROCESSORS_ONLN);

    // leitura das imagens a processar //
    Imagens = fopen(list, "r");
    if (Imagens == NULL)
    {
        exit(0);
    }
    else
    {

        while (fscanf(Imagens, "%s", linha) != EOF)
        {
            N_imagens++;
        }
        lista_i = NULL;
        lista_i = (char **)malloc(N_imagens * sizeof(char *));
        if (lista_i == NULL)
        {
            exit(0);
        }
    }
    fclose(Imagens);
    Imagens = fopen(list, "r");
    if (Imagens == NULL)
    {
        exit(0);
    }
    else
    {
        i = 0;
        while (fscanf(Imagens, "%s", linha) != EOF)
        {
            lista_i[i] = NULL;
            int a = strlen(linha);
            lista_i[i] = malloc((a + 1) * sizeof(char));
            if (lista_i[i] == NULL)
            {
                exit(0);
            }
            strcpy(lista_i[i], linha);
            i++;
        }
    }
    fclose(Imagens);

    // Distribuicao das imagens pelas threads
    int a = N_imagens / Num_threads;
    int b = N_imagens % Num_threads;
    Imagens_pt = NULL;
    Imagens_pt = malloc(Num_threads * sizeof(int));
    if (Imagens_pt == NULL)
    {
        exit(0);
    }
    for (i = 0; i < Num_threads; i++)
    {
        Imagens_pt[i] = 0;
    }
    for (i = 0; i < Num_threads; i++)
    {
        if (i < b)
        {
            Imagens_pt[i] = a + 1;
        }
        else
        {
            Imagens_pt[i] = a;
        }
    }

    // Criacao e inicializacao das estruturas para os argumentos

    N = 0;
    argumentos = NULL;
    argumentos = (struct Argumentos **)malloc(Num_threads * sizeof(struct Argumentos *));
    if (argumentos == NULL)
    {
        exit(0);
    }

    for (i = 0; i < Num_threads; i++)
    {
        argumentos[i] = NULL;
        argumentos[i] = (struct Argumentos *)malloc(sizeof(struct Argumentos));
        if (argumentos[i] == NULL)
        {
            exit(0);
        }
        argumentos[i]->q = i;
        argumentos[i]->min = N;
        N += (Imagens_pt[i] - 1);
        argumentos[i]->max = N;
        N++;

        argumentos[i]->lista = NULL;
        argumentos[i]->lista = malloc(Imagens_pt[i] * sizeof(char *));
        if ((argumentos[i]->lista) == NULL)
        {
            exit(0);
        }
        int b = 0;
        for (int w = (argumentos[i]->min); w <= (argumentos[i]->max); w++)
        {
            // argumentos[i]->lista[w] = NULL;

            argumentos[i]->lista[b] = malloc((strlen(lista_i[w]) + 1) * sizeof(char));
            if ((argumentos[i]->lista[b]) == NULL)
            {
                exit(0);
            }
            strcpy((argumentos[i]->lista[b]), lista_i[w]);
            b++;
        }
    }

    for (i = 0; i < N_imagens; i++)
    {
        free(lista_i[i]);
    }
    free(lista_i);
    free(Imagens_pt);

    /* creation of output directories */
    if (create_directory(RESIZE_DIR) == 0)
    {
        fprintf(stderr, "Impossible to create %s directory\n", RESIZE_DIR);
        exit(-1);
    }
    if (create_directory(THUMB_DIR) == 0)
    {
        fprintf(stderr, "Impossible to create %s directory\n", THUMB_DIR);
        exit(-1);
    }
    if (create_directory(WATER_DIR) == 0)
    {
        fprintf(stderr, "Impossible to create %s directory\n", WATER_DIR);
        exit(-1);
    }

    // Criacao das threads
    // thread_id = NULL;
    thread_id = (pthread_t *)malloc(Num_threads * sizeof(pthread_t));
    if (thread_id == NULL)
    {
        exit(0);
    }

    for (i = 0; i < Num_threads; i++)
    {

        pthread_create(&thread_id[i], NULL, Processo, argumentos[i]);
    }

    // Espera pela conclusao das threads
    i = 0;
    while (i < Num_threads)
    {
        pthread_join(thread_id[i], NULL);
        i++;
    }

    free(thread_id);
    for (int i = 0; i < Num_threads; i++)
    {
        int min = (argumentos[i])->min;
        int max = (argumentos[i])->max;
        int c = max - min + 1;
        for (int x = 0; x < c; x++)
        {
            free((argumentos[i])->lista[x]);
        }
        free((argumentos[i])->lista);
        free(argumentos[i]);
    }
    free(argumentos);
}
