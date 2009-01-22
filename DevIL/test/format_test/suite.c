#include "config.h"
#include <IL/il.h>
#include <IL/ilu.h>

#include <alloca.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef WORDS_BIGENDIAN
enum colors {ALPHA, BLUE, GREEN, RED};
#else /* not WORDS_BIGENDIAN */
enum colors {RED, GREEN, BLUE, ALPHA};
#endif /* not WORDS_BIGENDIAN */

enum test_results {TEST_OK = 0, TEST_FAIL = 0x1, TEST_FAIL_QUANTIL = 0x2, TEST_FAIL_INTEGRAL = 0x4 };

enum {ACTION_HELP = 0x1, ACTION_VERBOSE = 0x2, ACTION_PRESERVE_TESTFILE = 0x4};
enum {EXPECT_EXTENSION = 0x1, EXPECT_QUANTILE = 0x2};

int verbose = 0;
int preserve_testfile = 0;
double quantile_spec = 0.65;

int compare_ILubyte (const void * a, const void * b)
{ return ( *(ILubyte * )a - *(ILubyte * )b ); }

struct PIXEL
{
	ILubyte color[4];
};
typedef struct PIXEL pixel;

const int bpp = 4;

void generate_test_image(pixel * data, int w, int h)
{
	int i, j, index;
	/* For easier notations */
	int h_5 = h / 5;
	for (j = 0, index = 0; j < h_5 * 3; j++)
		for (i = 0; i < w; i++)
		{/* Making 27 samples from the RGB cube. */
			index = 9 * (j / h_5) + (i * 9) / w;
			data[j * w + i].color[RED] = 255 / 6 + (255 / 3) * (index % 3) ;
			data[j * w + i].color[GREEN] = 255 / 6 + (255 / 3) * ((index % 9) / 3 );
			data[j * w + i].color[BLUE] = 255 / 6 + (255 / 3) * (index / 9);
			data[j * w + i].color[ALPHA] = 255 - 63 * ((j * 4 / h_5) % 4);
		}
	double red, green, blue, sum;
	for (j = 3 * h_5; j < 4 * h_5; j++)
		for (i = 0; i < w; i++)
		{/* Making color gradient */
			red = cos(2 * M_PI / (double)w * i) / 2.0 + 0.5;
			green = cos(2 * M_PI / (double)w * i + M_PI * 1.0 / 3.0) / 2.0 + 0.5;
			blue = cos(2 * M_PI / (double)w * i + M_PI * 2.0 / 3.0) / 2.0 + 0.5;
			sum = sqrt(red * red + green * green + blue * blue);
			data[j * w + i].color[RED] = 255 * red / sum;
			data[j * w + i].color[GREEN] = 255 * green / sum;
			data[j * w + i].color[BLUE] = 255 * blue / sum;
			data[j * w + i].color[ALPHA] = 255;
		}
	for (j = 4 * h_5; j < 5 * h_5; j++)
		for (i = 0; i < w; i++)
		{/* Making smooth B&W gradient */
			data[j * w + i].color[RED] = (i * 255) / w;
			data[j * w + i].color[GREEN] = (i * 255) / w;
			data[j * w + i].color[BLUE] = (i * 255) / w;
			data[j * w + i].color[ALPHA] = 255;
		}
}

int save_test_image(const char * name, int w, int h)
{
	/* Set constants so that there are no div rounding errors */
	ILuint handle;
	/* Generate the paperwork for our test image... */
	ilGenImages(1, & handle);
	ilBindImage(handle);
	/* how much memory will we need? */
	int memory_needed = w * h * sizeof(pixel);
	pixel * data = (pixel * )malloc(memory_needed);
	if (data == NULL)
	{
		fprintf(stderr, "Out of memory, %s:%d\n", __FILE__, __LINE__);
		return IL_OUT_OF_MEMORY;
	}

	generate_test_image(data, w, h);

	/* finally set the image data */
	ilTexImage(w, h, 1, bpp, IL_RGBA, IL_UNSIGNED_BYTE, data);
	/* and dump them to the disc... */
	ILboolean saved = ilSaveImage(name);
	int return_value = IL_NO_ERROR;
	if (saved == IL_FALSE)	
	{
		return_value = ilGetError();
		fprintf(stderr, "Error saving file '%s'\nReason: %s\n", name, iluErrorString(return_value));
	}
	/* Finally, clean the mess! */
	ilDeleteImages(1, & handle);
	free(data);
	return return_value;
}

/**
 * Returns the sum of absolute diferences and stuff..
 */
int calculate_median_pixel_distance_abs(ILubyte * reference, ILubyte * sample, int num_subpixels, double quantile)
{
	ILubyte * differences = (ILubyte *)malloc(sizeof(ILubyte) * num_subpixels);	
	if (differences == NULL)
	{
		fprintf(stderr, "Out of memory, %s:%d\n", __FILE__, __LINE__);
		return IL_OUT_OF_MEMORY;
	}
	int ii, difference;
	for (ii = 0; ii < num_subpixels; ii++)
	{/* Going through all pixel components and assigning the absolute difference to differences array */
		difference = sample[ii] - reference[ii];	/* avoiding overflow + need to have abs() */
		differences[ii] = (difference >= 0) ? difference : - difference;
	}
	/* We need the quantile, so we have to sort the array */
	qsort (differences, num_subpixels, sizeof(ILubyte), compare_ILubyte);
	if (quantile > 1.0 || quantile < 0.0)	/* The quantile has totally wrong value... */
	{
		fprintf(stderr, "Come on... Quantile should be between 0.0 and 1.0 (you wanted %f)\n", quantile);
		return -1;
	}
	int index = (int)(quantile * num_subpixels);	/* Which member of the sorted array is the quantile? */
	int result = differences[index];
	free(differences);	/* Clean the mess */
	differences = 0;
	return result;
}

/**
 * Returns the sum of squared differences between component color values, normed.
 */
double calculate_averaged_distance_sqr_integral(ILubyte * reference, ILubyte * sample, int num_subpixels)
{
	double differences = 0;
	int ii, difference;
	for (ii = 0; ii < num_subpixels; ii++)
	{/* Going through all pixel components and assigning the absolute difference to differences array */
		difference = sample[ii] - reference[ii];	/* avoiding overflow + need to have abs() */
		differences += difference * difference;
	}
	return sqrt(differences) / (double)num_subpixels;
}

int test_format(const char * suffix)
{
	const char * base_name = "test.";
	char filename [64];
	if (strlen(suffix) + strlen(base_name) >= sizeof(filename))
		return -1; /* buffer overflow */
	sprintf(filename, "%s%s", base_name, suffix);
	int saved = save_test_image(filename, 603, 300);
	if (saved != 0) /* something went wrong, no point in continuing, the user already knows */
		return saved;
	/* First, we load the generated and saved image */
	ILuint handle;
	ilGenImages(1, & handle);
	ilBindImage(handle);
	ilLoadImage(filename);
	/* getting image width and height */
	int w, h;
	w = ilGetInteger(IL_IMAGE_WIDTH);  
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	/* Then we save image data into arrays that we allocate now */
	ILubyte * reference, * sample;
	reference = malloc(w * h * bpp * sizeof(ILubyte));
	sample = malloc(w * h * bpp * sizeof(ILubyte));
	if (reference == NULL || sample == NULL)
	{
		if (preserve_testfile == 0)
			remove(filename);
		fprintf(stderr, "Out of memory, %s:%d\n", __FILE__, __LINE__);
		return IL_OUT_OF_MEMORY;
	}
	/* Allocation went fine and now we get what SHOULD BE on the loaded image... */
	generate_test_image((pixel *)reference, w, h);
	ilCopyPixels(0, 0, 0, w, h, 1, IL_RGBA, IL_UNSIGNED_BYTE, sample);

	const int quantile_treshold = 2; 
	int quantile = calculate_median_pixel_distance_abs(reference, sample, w *  h * bpp, quantile_spec);
	int return_value = 0;
	if (quantile > quantile_treshold)
	{
		printf("Quantile test failed: More than %d%% of subpixels have greater error than %d\n", (int)(100 - 100 * quantile_spec), quantile_treshold);
		return_value |= TEST_FAIL_QUANTIL | TEST_FAIL;
	}

	const double integral_treshold = 0.5;
	double integral = calculate_averaged_distance_sqr_integral(reference, sample, w *  h * bpp);
	if (integral > integral_treshold)
	{
		printf("Integral test failed: Average deviation is %f, should be below %f\n", integral, integral_treshold);
		return_value |= TEST_FAIL_INTEGRAL | TEST_FAIL;
	}

	if (verbose)
		printf("Testing file '%s'. \nQuantile[%.3f]: %d (max %d)\nIngegral: %f (max %f)\n", filename, quantile_spec, quantile, quantile_treshold, integral, integral_treshold);

	free(sample);
	free(reference);
	/* We do not want to keep the saved test image by default... */
	if (preserve_testfile == 0)
		remove(filename);
	return return_value;
}

/**
 */
void parse_commandline(int argc, char ** argv, char ** extension)
{
	int actions = 0, expectations = 0;
	const int long_str = 32;
	int i;
	for (i = 1; i < argc; i++)
	{
		actions = 0;
		/* Find out what to do and what to expect */
		if (argv[i][0] == '-')
		{/* Ho, an option was passed... */
			if (argv[i][1] == '-')
			{/* Deal with long options */
				if(strncmp(argv[i], "--extension", long_str))
				{
					expectations |= EXPECT_EXTENSION;
				}
				else if (strncmp(argv[i], "--quantile", long_str))
				{
					expectations |= EXPECT_QUANTILE;
				}
				else if (strncmp(argv[i], "--help", long_str))
				{
					actions |= ACTION_HELP;
				}
				else if (strncmp(argv[i], "--verbose", long_str))
				{
					actions |= ACTION_VERBOSE;
				}
				else if (strncmp(argv[i], "--preserve", long_str))
				{
					actions |= ACTION_PRESERVE_TESTFILE;
				}

			}
			else /* if not (argv[i][1] == '-') */
			{/* Deal with short options */
				int j;
				for (j = 1; j < strlen(argv[i]); j++)
					switch(argv[i][j])
					{
						case 'h':
						case '?':
							actions |= ACTION_HELP;
							break;
						case 'v':
							actions |= ACTION_VERBOSE;
							break;
						case 'e':
							expectations |= EXPECT_EXTENSION;
							break;
						case 'q':
							expectations |= EXPECT_QUANTILE;
							break;
						case 'p':
							actions |= ACTION_PRESERVE_TESTFILE;
							break;
					}/* end switch(argv[i][j]) */
			}/* if not (argv[i][1] == '-') */
		} 
		else /* (! argv[i][0] == '-') */
		{/* Now handle expectations... */
			if (expectations & EXPECT_EXTENSION)
			{/* return index of extension */
				* extension = argv[i];
			}
			else if (expectations & EXPECT_QUANTILE)
			{
				sscanf(argv[i], "%lf", & quantile_spec);
			}
			expectations = 0;
		}/* (! argv[i][0] == '-') */
		/* now handle actions... */
		if (actions & ACTION_VERBOSE)
		{
			verbose++;
		}
		if (actions & ACTION_HELP)
		{
			printf("This will be helpful one day...\n");
		}
		if (actions & ACTION_PRESERVE_TESTFILE)
		{
			preserve_testfile = 1;
		}
	}/* endfor <going through argv> */
}

int main(int argc, char ** argv)
{
	/* has to be done */
	ilInit();
	iluInit();
	/* Consistent loading stuff... */
	ilEnable(IL_ORIGIN_SET);
	char * extension = NULL;
	char ** extension_ptr = & extension;
	
	parse_commandline(argc, argv, extension_ptr);
	int return_value;
	if (extension != NULL)
	{
		return_value = test_format(extension);
		if (return_value != 0)
			fprintf(stderr, "FAIL (.%s)\n", extension);
	}
	return return_value;
}

