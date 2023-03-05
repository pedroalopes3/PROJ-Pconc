/******************************************************************************
 * Programacao Concorrente
 * MEEC 21/22
 *
 * Projecto - Parte1
 *                           serial-complexo.c
 *
 * Compilacao: gcc serial-complexo -o serial-complex -lgd
 *
 *****************************************************************************/

#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include "image-lib.h"
#include "generate-resize.h"

/* the directories wher output files will be placed */
#define RESIZE_DIR "Resize-dir/"

/******************************************************************************
 * main()
 *
 * Arguments: (none)
 * Returns: 0 in case of sucess, positive number in case of failure
 * Side-Effects: creates thumbnail, resized copy and watermarked copies
 *               of images
 *
 * Description: implementation of the complex serial version
 *              This application only works for a fixed pre-defined set of files
 *
 *****************************************************************************/

void *Resize(void *arg)
{

	int index = *(int*) arg;
	
/* image created and to be saved on disk	 */
	char out_file_name[150];
	char in_file_name[150];


	/* input images */
	
	
	
	/* output images */
	gdImagePtr out_watermark_img = NULL ,out_resized_img;


	/* Iteration over all the files to add the watermarks*/
    int N_imagens,b = 1; 
	int size = 0;
	char directoria[50]={'0'};

    read(in[index][0],&N_imagens,sizeof(int));
	read(in[index][0],&size,sizeof(int));
    read(in[index][0],directoria,sizeof(char) * size);
	write(in[index+threads_Est][1],&N_imagens,sizeof(int));
    write(in[index+threads_Est][1],&size,sizeof(int));
    write(in[index+threads_Est][1],directoria,sizeof(char) * size);
	
	
	char imagem[50] = {'0'};
	while(b <= N_imagens)
	{
        
		
		read(in[index][0], &size, sizeof(int));
        
        
		read(in[index][0], imagem, sizeof(char) *size);
		read(in[index][0],&out_watermark_img,sizeof(gdImagePtr));
		
        
		
		sprintf(out_file_name, "%s%s%s",directoria, RESIZE_DIR, imagem);
		if (access(out_file_name, F_OK) == 0)
		{
			write(in[index+threads_Est][1], &size, sizeof(int));
        	write(in[index+threads_Est][1], imagem, (size) * sizeof(char));
        	write(in[index+threads_Est][1],&out_watermark_img, sizeof(gdImagePtr));
        	b++;
			continue;
		}
		printf("resize %s\n", imagem);
		/* load of the input file */
		
		
			out_resized_img = resize_image(out_watermark_img, 800);
			if (out_resized_img == NULL)
			{
				fprintf(stdin, "Impossible to resize %s image\n", in_file_name);
			}
			else
			{
				/* save resized */
				
				if (write_png_file(out_resized_img, out_file_name) == 0)
				{
					fprintf(stdin, "Impossible to write %s image\n", out_file_name);
				}
				gdImageDestroy(out_resized_img);
			}
			
		
		
	
		// comunicacao com proximo estagio
		write(in[index+threads_Est][1], &size, sizeof(int));
        write(in[index+threads_Est][1], imagem, (size) * sizeof(char));
        write(in[index+threads_Est][1], &out_watermark_img, sizeof(gdImagePtr));
        
            
        b++;
		}
	


	return (0);
	}
