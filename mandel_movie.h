/**********************************************************
 * @file mandel_movie.h
 * @brief header function containing struct
 * 
 * Author: Hunter Carroll
 * Course: CPE 2600
 * Section: 012
 * Assignment Lab 12
**********************************************************/
#ifndef MANDEL_MOVIE_h
#define MANDEL_MOVIE_h

#include "jpegrw.h"

// struct for thread data
typedef struct {
    imgRawImage* img;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int max;
    int start_row;
    int end_row;
} thread_data_t;

#endif