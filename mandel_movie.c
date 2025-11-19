/*************************************************************************
 * @file mandel_movie.c
 * @brief uses child processes to generate 50 images by progressively
 * changing one or more image parameters (scale)
 *
 * Name: Hunter Carorll
 * Course: CPE 2600
 * Section: 012
 * Assignment: Lab 11 - Multiprocessing
 * Date: 11/12/2025
 *
 * Compile: make
 * Run: ./mandel_movie
*************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>
#include "jpegrw.h"

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
static void show_help();

int main(int argc, char *argv[])
{
    char c;

    // Default value if no command line parameter given
    const char *outfile = "mandel_movie.mpg";
    int num_child_processes = 1;
    
    // constants and variables
    int frames = 50;
    double xcenter = 0.0;
    double ycenter = 0.0;
    double xscale = 4;
    int image_width = 1000;
    int image_height = 1000;
    int max = 1000;

    // Use getopt to identify command line parameter
    while((c = getopt(argc,argv,"x:y:s:W:H:m:n:o:h"))!=-1) {
		switch(c) 
		{
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				xscale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
            case 'n':
                num_child_processes = atoi(optarg);
                break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

	// Calculate frames per process
	int frames_per_process = frames / num_child_processes;
	int remainder_frames = frames % num_child_processes;
	
	// Calculate scale range for zoom effect
	double start_scale = xscale;
	double end_scale = xscale * 0.01;  // Zoom in by 100x
	double scale_factor = end_scale / start_scale;

    for(int i = 0; i < num_child_processes; i++) {
		int pid = fork();

		if (pid < 0) {
			// Fork failed
			perror("fork");
			exit(1);
		}

		if (pid == 0) {
			// child calculates which frames it will handle
			int start_frame = i * frames_per_process + (i < remainder_frames ? i : remainder_frames);
			int end_frame = start_frame + frames_per_process + (i < remainder_frames ? 1 : 0);

			// Generate each frame assigned to this child
			for(int frame = start_frame; frame < end_frame; frame++) {
				// Calculate scale for this frame (exponential zoom for smooth effect)
				double frame_ratio = (double)frame / (frames - 1);
				double current_xscale = start_scale * pow(scale_factor, frame_ratio);
				
				// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
				double current_yscale = current_xscale / image_width * image_height;

				// Create filename for this frame
				char frame_filename[256];
				sprintf(frame_filename, "frame_%03d.jpg", frame);

				// Display the configuration of the image.
				printf("mandel: x=%lf y=%lf xscale=%lf yscale=%lf max=%d outfile=%s\n",
				       xcenter, ycenter, current_xscale, current_yscale, max, frame_filename);

				// create image of appropriate size
				imgRawImage* img = initRawImage(image_width, image_height);

				// fill with black
				setImageCOLOR(img, 0);

				// compute mandel image
				compute_image(img, 
				             xcenter - current_xscale/2, xcenter + current_xscale/2,
				             ycenter - current_yscale/2, ycenter + current_yscale/2,
				             max);

				// save image in stated file
				storeJpegImageFile(img, frame_filename);

				// free the memory
				freeRawImage(img);
			}
			
			// Child exits after generating all frames assigned
			exit(0);
		}
		// Parent continues the loop to fork more children
	}
	
	// parent waits for all children to complete
	for(int i = 0; i < num_child_processes; i++) {
		wait(NULL);
	}
	
	printf("All %d frames generated. Use ffmpeg in Ubuntu to create the movie:\n", frames);
	printf("ffmpeg -y -r 10 -i frame_%%03d.jpg -c:v libx264 -pix_fmt yuv420p %s\n", outfile);

	return 0;
}




/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max )
{
	int i,j;

	int width = img->width;
	int height = img->height;

	// For every pixel in the image...

	for(j=0;j<height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,max));
		}
	}
}


/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0x9B111E*iters/(double)max;
	return color;
}


// Show help message
void show_help()
{
	printf("Use: mandel_movie [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-n <num>    Number of child processes to use. (default=1)\n");
	printf("-o <file>   Set output file. (default=mandel_movie.mpg)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel_movie -n 4\n");
	printf("mandel_movie -n 8 -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel_movie -n 2 -x -.38 -y -.665 -s .05 -m 100\n\n");
}
