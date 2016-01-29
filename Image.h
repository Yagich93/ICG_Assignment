/* Image.h*/
/* BMP loading and manipulation. */

/* Image sturcture. */
typedef struct {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
} Image;

/* Loading BMP image. */
int ImageLoad(char *filename, Image *image);