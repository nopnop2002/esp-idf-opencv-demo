/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/core/utility.hpp>

#include "opencv2/opencv_modules.hpp"

#include "opencv2/core.hpp"
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/softfloat.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>

#include <iostream>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace std;
using namespace cv;

extern "C" void cv_swap_image(uint8_t* data, int dlen, int width, int heigth, int swap_size, int swap_position);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

void cv_swap_image(uint8_t* data, int dlen, int width, int heigth, int swap_size, int swap_position)
{
	printf("OpenCV swap image.\n");
	cv::Mat src( heigth, width, CV_16UC1, (void*)data );
	cv::Mat cloned = src.clone();
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	int swap_x1 = 0;
	int swap_y1 = 0;
	int swap_x2 = src.cols - swap_size;
	int swap_y2 = swap_y1;

	// Swap position(Top left X coordinate, Y coordinate)
	if (swap_position == 1) {
		swap_y1 = (rows / 2) - (swap_size / 2);
		swap_y2 = swap_y1;
	} else if (swap_position == 2) {
		swap_y1 = rows - swap_size;
		swap_y2 = swap_y1;
	} else if (swap_position == 3) {
		swap_x1 = (cols / 2) - (swap_size / 2);
		swap_x2 = swap_x1;
		swap_y2 = rows - swap_size;
	}

	// Crop image
	cv::Rect roi1 = cv::Rect(swap_x1, swap_y1, swap_size, swap_size); 
	cv::Mat image_roi1 = cloned(roi1);
	cv::Rect roi2 = cv::Rect(swap_x2, swap_y2, swap_size, swap_size); 
	cv::Mat image_roi2 = cloned(roi2);
	cv::Mat image_roi3 = src(roi2);

	// Swap image
	image_roi1.copyTo(image_roi2);
	image_roi3.copyTo(image_roi1); 

	std::memcpy(data, cloned.data, dlen);
}
