/* the directories wher output lista will be placed */
#define RESIZE_DIR "./serial-Resize/"
#define THUMB_DIR "./serial-Thumbnail/"
#define WATER_DIR "./serial-Watermark/"



typedef struct Argumentos{
    int q;
    int min;               // do intervalo das imagens da lista a serem   
    int max;               // tratadas por uma thread
    char **lista;  
}Args;

void* Processo(void* arg);