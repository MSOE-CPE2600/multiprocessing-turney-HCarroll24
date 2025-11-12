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
#include "jpegrw.h"

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
    double yscale = 0; // calculate later
    int image_width = 1000;
    int image_height = 1000;
    int max = 1000;
    double scale = 1.0;
    char cmd[512];

    // Use getopt to identify command line parameter
    while((c = getopt(argc, argv, "n:o")) != -1) {
        switch(c)
        {
            case 'n':
                num_child_processes = atof(optarg);
                break;
            
            case 'o':
                outfile = optarg;
                break;
        }
    }

    // Call mandel from command line to generate image
    

}