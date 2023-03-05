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
#include "generate-watermark.h"

/* the directories wher output files will be placed */
#define WATER_DIR "./Watermark-dir/"

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
void *Watermark(void *arg)
{

	int min = ((struct Argumentos *)arg)->min;
	int max = ((struct Argumentos *)arg)->max;

	int c = max - min;

	/* file name of the image created and to be saved on disk	 */
	char out_file_name[100];

	/* input images */
	gdImagePtr in_img, watermark_img;
	/* output images */
	gdImagePtr out_watermark_img;

	watermark_img = ((struct Argumentos *)arg)->watermark_img;

	/* Iteration over all the files to add the watermarks
	 */
	for (int K = 0; K <= c; K++)
	{

		/* save watermark */
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
			fprintf(stderr, "Impossible to read %s image\n", ((struct Argumentos *)arg)->lista[K]);
			continue;
		}

		/* add watermark */
		out_watermark_img = add_watermark(in_img, watermark_img);
		if (out_watermark_img == NULL)
		{
			fprintf(stderr, "Impossible to creat thumbnail of %s image\n", ((struct Argumentos *)arg)->lista[K]);
		}
		else
		{

			if (write_png_file(out_watermark_img, out_file_name) == 0)
			{
				fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			}
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);
	}
	return (0);
}
