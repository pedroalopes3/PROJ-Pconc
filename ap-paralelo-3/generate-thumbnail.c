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

#include "image-lib.h"
#include <string.h>
#include <unistd.h>
#include "generate-thumbnail.h"

/* the directories wher output files will be placed */
#define THUMB_DIR "Thumbnail-dir/"

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
void *Thumbnail(void *arg)
{

	/* file name of the image created and to be saved on disk	 */
	char out_file_name[150];
	char in_file_name[150];

	int index = *(int*) arg;
	

	/* input images */
	
	
	/*output images */ 
	gdImagePtr out_thumb_img , out_watermark_img =NULL;
	

	
	/* 2nd iteration over all the files
	 * To create the thumbnails
	 */
	 int N_imagens, c = 1;
	 int size=0;
	 char directoria[50]={'0'};

	read(in[index][0],&N_imagens,sizeof(int));
	read(in[index][0],&size,sizeof(int));
    read(in[index][0],directoria,sizeof(char) * size);


	
	char imagem[50]={'0'};
	while(c <= N_imagens)
	{
		
		read(in[index][0],&size,sizeof(size));
		
		
		read(in[index][0],imagem,sizeof(char)*size);
		read(in[index][0],&out_watermark_img,sizeof(gdImagePtr));
		
        
		sprintf(out_file_name, "%s%s%s",directoria, THUMB_DIR, imagem);
		if (access(out_file_name, F_OK) == 0)
		{
			c++;
			gdImageDestroy(out_watermark_img);
			continue;
		}
		printf("thumbnail %s\n", imagem);
		/* load of the input file */
		

		
			out_thumb_img = make_thumb(out_watermark_img, 150);
			if (out_thumb_img == NULL)
			{
				fprintf(stdin, "Impossible to creat thumbnail of %s image\n", in_file_name);
			}
			else
			{
				/* save thumbnail image */
				
				if (write_png_file(out_thumb_img, out_file_name) == 0)
				{
					fprintf(stdin, "Impossible to write %s image\n", out_file_name);
				}
				gdImageDestroy(out_thumb_img);
			}
			gdImageDestroy(out_watermark_img);
		
		
		
		c++;
		
		
	}
	return (0);
}
