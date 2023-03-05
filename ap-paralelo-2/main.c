#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "generate-watermark.h"
#include "generate-resize.h"
#include "generate-thumbnail.h"
#include "image-lib.h"

/* the directories wher output files will be placed */
#define WATER_DIR "./Watermark-dir/"
/* the directories wher output files will be placed */
#define RESIZE_DIR "./Resize-dir/"
/* the directories wher output files will be placed */
#define THUMB_DIR "./Thumbnail-dir/"

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
    
    struct Argumentos *argumentos;
    pthread_t *thread_id;
    void *thread_ret;
    int N_cores;

    //////////////////////////////////////////////////////

    strcpy(diretoria, argv[1]);
    strcpy(list, argv[1]);
    strcat(list, "/image-list.txt");

    

    Num_threads = 3;

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

    // Criacao e inicializacao das estruturas para os argumentos

    N = 0;
    argumentos = NULL;
    argumentos = (struct Argumentos *)malloc(sizeof(struct Argumentos));
    if (argumentos == NULL)
    {
        exit(0);
    }

    argumentos->min = N;
    N += (N_imagens - 1);
    argumentos->max = N;

    argumentos->lista = NULL;
    argumentos->lista = malloc(N_imagens * sizeof(char *));
    if ((argumentos->lista) == NULL)
    {
        exit(0);
    }
    int b = 0;
    for (int w = (argumentos->min); w <= (argumentos->max); w++)
    {
        // argumentos[i]->lista[w] = NULL;

        argumentos->lista[b] = malloc((strlen(lista_i[w]) + 1) * sizeof(char));
        if ((argumentos->lista[b]) == NULL)
        {
            exit(0);
        }
        strcpy((argumentos->lista[b]), lista_i[w]);
        b++;
    }
    argumentos->watermark_img = read_png_file("watermark.png");
	if (argumentos->watermark_img == NULL)
	{
		fprintf(stdin, "Impossible to read %s water\n", "watermark.png");
		exit(-1);
	}
	;

    for (i = 0; i < N_imagens; i++)
    {
        free(lista_i[i]);
    }
    free(lista_i);
    

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

    

    pthread_create(&thread_id[0], NULL, Watermark, argumentos);
    pthread_create(&thread_id[1], NULL, Resize, argumentos);
    pthread_create(&thread_id[2], NULL, Thumbnail, argumentos);

    // Espera pela conclusao das threads
    
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_join(thread_id[2], NULL); 

    free(thread_id);

    int min = (argumentos)->min;
    int max = (argumentos)->max;
    int c = max - min+1;
    for (int x = 0; x < c; x++)
    {
        free((argumentos)->lista[x]);
    }
    gdImageDestroy((argumentos)->watermark_img);
    free((argumentos)->lista);
    free(argumentos);
}
