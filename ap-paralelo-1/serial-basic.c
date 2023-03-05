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

#include "gd.h"
#include "image-lib.h"
#include "serial-basic.h"
#include <string.h>
#include <unistd.h>



/******************************************************************************
 * main()
 *
 * Arguments: (none)
 * Returns: 0 in case of sucess, positive number in case of failure
 * Side-Effects: creates thumbnail, resized copy and watermarked copies
 *               of images
 *
 * Description: implementation of the complex serial version
 *              This application only works for a fixed pre-defined set of lista
 *
 *****************************************************************************/
void *Processo(void *arg)
{

	int min = ((struct Argumentos *)arg)->min;
	int max = ((struct Argumentos *)arg)->max;
	int thread = ((struct Argumentos *)arg)->q;
	int c = max-min;

	/* file name of the image created and to be saved on disk	 */
	char out_file_name[100];

	

	
	/* input images */
	gdImagePtr in_img, watermark_img;
	/* output images */
	gdImagePtr out_thumb_img, out_resized_img, out_watermark_img;

	

	watermark_img = read_png_file("watermark.png");
	if (watermark_img == NULL)
	{
		fprintf(stdin, "Impossible to read %s water\n", "watermark.png");
		exit(-1);
	}
	

	
	

	/*
	 * Add the watermarks
	 */
	
	for (int K = 0; K <= c; K++)
	{
		sprintf(out_file_name, "%s%s", WATER_DIR, ((struct Argumentos *)arg)->lista[K]);
		if (access(out_file_name, F_OK) == 0)
		{
			continue;
		}
		printf("watermark  %s\n", ((struct Argumentos *)arg)->lista[K]);
		/* load of the input file */
		in_img = read_png_file(((struct Argumentos *)arg)->lista[K]);
		if (in_img == NULL)
		{
			fprintf(stdin, "Impossible to read  %d %s image\n",K, ((struct Argumentos *)arg)->lista[K]);
			continue;
		}

		/* add watermark */
		out_watermark_img = add_watermark(in_img, watermark_img);
		if (out_watermark_img == NULL)
		{
			fprintf(stdin, "Impossible to creat thumbnail of %s image\n",((struct Argumentos *)arg)->lista[K]);
		}
		else
		{
			/* save watermark */
			
			if (write_png_file(out_watermark_img, out_file_name) == 0)
			{
				fprintf(stdin, "Impossible to write %s image\n", out_file_name);
			}
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);
	}

	/*
	 * To resize images and add watermark
	 */
	for (int K = 0; K <= c; K++)
	{

		sprintf(out_file_name, "%s%s", RESIZE_DIR, ((struct Argumentos *)arg)->lista[K]);
		if (access(out_file_name, F_OK) == 0)
		{
			continue;
		}
		printf("%d resize %s\n", thread,((struct Argumentos *)arg)->lista[K]);
		/* load of the input file */
		in_img = read_png_file(((struct Argumentos *)arg)->lista[K]);
		if (in_img == NULL)
		{
			fprintf(stdin, "Impossible to read %s image\n", ((struct Argumentos *)arg)->lista[K]);
			continue;
		}
		/* resizes of each image */
		out_watermark_img = add_watermark(in_img, watermark_img);
		if (out_watermark_img == NULL)
		{
			fprintf(stdin, "Impossible to add watermark to %s image\n", ((struct Argumentos *)arg)->lista[K]);
		}
		else
		{
			out_resized_img = resize_image(out_watermark_img, 800);
			if (out_resized_img == NULL)
			{
				fprintf(stdin, "Impossible to resize %s image\n", ((struct Argumentos *)arg)->lista[K]);
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
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);
	}

	/*
	 * Add watermark and create thumbnails
	 */
	for (int K = 0; K <= c; K++)
	{
        sprintf(out_file_name, "%s%s", THUMB_DIR, ((struct Argumentos *)arg)->lista[K]);
		if (access(out_file_name, F_OK) == 0)
		{
			continue;
		}
		printf("%d thumbnail %s\n", thread,((struct Argumentos *)arg)->lista[K]);
		/* load of the input file */
		in_img = read_png_file(((struct Argumentos *)arg)->lista[K]);
		if (in_img == NULL)
		{
			fprintf(stdin, "Impossible to read %s image\n", ((struct Argumentos *)arg)->lista[K]);
			continue;
		}

		/* creation of thumbnail image */
		out_watermark_img = add_watermark(in_img, watermark_img);
		if (out_watermark_img == NULL)
		{
			fprintf(stdin, "Impossible to creat thumbnail of %s image\n", ((struct Argumentos *)arg)->lista[K]);
		}
		else
		{
			out_thumb_img = make_thumb(out_watermark_img, 150);
			if (out_thumb_img == NULL)
			{
				fprintf(stdin, "Impossible to creat thumbnail of %s image\n",((struct Argumentos *)arg)->lista[K]);
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
		}
		gdImageDestroy(in_img);
	}

	gdImageDestroy(watermark_img);
	return(0);
}
