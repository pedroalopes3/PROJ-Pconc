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
#define WATER_DIR "Watermark-dir/"
/* the directories wher output files will be placed */
#define RESIZE_DIR "Resize-dir/"
/* the directories wher output files will be placed */
#define THUMB_DIR "Thumbnail-dir/"

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
    int* Imagens_pt;
    
    struct Argumentos *argumentos;
    pthread_t *thread_id;
    void *thread_ret;
    int N_cores;
    

    //////////////////////////////////////////////////////

    
    strcpy(diretoria, argv[1]);
    strcpy(list, argv[1]);
    strcat(list, "/image-list.txt");

    if(diretoria[strlen(diretoria) - 1] != '/'){
    strcat(diretoria,"/");}

    // Convercao do argumento para int (assumimos que argv[2] < 100)

    if (strlen(argv[2]) == 2)
    {
        Num_threads = ((argv[2][0]) - 48) * 10 + ((argv[2][1]) - 48);
    }
    else
    {
        Num_threads = (argv[2][0]) - 48;
    }
    threads_Est = Num_threads;
    Num_threads = Num_threads*3;

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

    //criaçao dos pipes
    in = (int **) malloc((Num_threads)*sizeof(int*));
    if(in==NULL){
        exit(0);
    }
    for(i=0;i<Num_threads;i++){
        in[i]= (int *) malloc(2*sizeof(int));
        if(in[i]==NULL){
            exit(0);
        }
        if(pipe(in[i])==-1){
            exit(0);
        }
    }
    
    char RESIZE_DIR1 [150]={'0'};
    char THUMB_DIR1 [150]={'0'};
    char WATER_DIR1 [150]={'0'};
    sprintf(RESIZE_DIR1, "%s%s",diretoria, RESIZE_DIR);
    sprintf(THUMB_DIR1, "%s%s",diretoria, THUMB_DIR);
    sprintf(WATER_DIR1, "%s%s",diretoria, WATER_DIR);
    if (create_directory(RESIZE_DIR1) == 0)
    {
        fprintf(stderr, "Impossible to create %s directory\n", RESIZE_DIR1);
        exit(-1);
    }
    if (create_directory(THUMB_DIR1) == 0)
    {
        fprintf(stderr, "Impossible to create %s directory\n", THUMB_DIR1);
        exit(-1);
    }
    if (create_directory(WATER_DIR1) == 0)
    {
        fprintf(stderr, "Impossible to create %s directory\n", WATER_DIR1);
        exit(-1);
    }

     
    // Criacao das threads
    // thread_id = NULL;
    thread_id = (pthread_t *)malloc(Num_threads * sizeof(pthread_t));
    if (thread_id == NULL)
    {
        exit(0);
    }

    int * counter = NULL;   
    counter = (int *)malloc(Num_threads * sizeof(int));
    if(counter == NULL){
        exit(0);}
        
    for(i=0;i<threads_Est;i++){
    counter[i]=i;
    pthread_create(&thread_id[i], NULL, Watermark,(void*)(counter+i));}

    for(i=threads_Est;i<2*threads_Est;i++){
    counter[i]=i;
    pthread_create(&thread_id[i], NULL, Resize,(void*)(counter+i));}

    for(i=2*threads_Est;i<3*threads_Est;i++){
    counter[i]=i;
    pthread_create(&thread_id[i], NULL, Thumbnail,(void*)(counter+i));}
    
    // calculo do numero de imagens por cada thread
    int divisao = N_imagens / threads_Est;
    int rest = N_imagens % threads_Est;
    Imagens_pt = NULL;
    Imagens_pt = malloc(threads_Est* sizeof(int));
    if (Imagens_pt == NULL)
    {
        exit(0);
    }
    for (i = 0; i < threads_Est; i++)
    {
        Imagens_pt[i] = 0;
    }
    for (i = 0; i < threads_Est; i++)
    {
        if (i < rest)
        {
            Imagens_pt[i] = divisao + 1;
        }
        else
        {
            Imagens_pt[i] = divisao;
        }
    }

    gdImagePtr watermark_img = read_png_file("watermark.png");
    if (watermark_img == NULL)
    {
        fprintf(stdin, "Impossible to read %s water\n", "watermark.png");
        exit(-1);
    };


    int size,a=0;
    size = strlen(diretoria) + 1;
    for(i=0;i<threads_Est;i++){
    write(in[i][1],&watermark_img,sizeof(gdImagePtr));
    write(in[i][1],(Imagens_pt+i),sizeof(int));
    write(in[i][1],&size,sizeof(int));
    write(in[i][1],diretoria,sizeof(char) * size);}
    
    
    
    for(i=0;i<N_imagens;i++){
        size = strlen(lista_i[i])+1;
        write(in[a][1], &size, sizeof(int));
        write(in[a][1], lista_i[i], size*sizeof(char));
        if(a==threads_Est-1) {
            a=0;
            }else{
                a++;}      
    }

    // Espera pela conclusao das threads
    
    for(int w=0;w<Num_threads;w++){
    pthread_join(thread_id[w], NULL);}
   
    free(thread_id);

     
    for(i=0;i<Num_threads;i++){
    close(in[i][0]);
    close(in[i][1]);
    free(in[i]);}
    free(in);

     for (i = 0; i < N_imagens; i++)
    {
        free(lista_i[i]);
    }
    gdImageDestroy(watermark_img);
    free(lista_i);
    free(counter);
    free(Imagens_pt);
}
