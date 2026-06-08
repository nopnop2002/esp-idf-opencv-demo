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

extern "C" void cv_crop_image(uint8_t* data, int dlen, int width, int heigth, int crop_size, int crop_position);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

void cv_crop_image(uint8_t* data, int dlen, int width, int heigth, int crop_size, int crop_position)
{
	printf("OpenCV crop image.\n");
	cv::Mat src( heigth, width, CV_16UC1, (void*)data );
	cv::Mat cloned = src.clone();
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	int crop_x = 0;
	int crop_y = 0;

	// Crop position(Top left X coordinate, Y coordinate)
	if (crop_position == 1) {
		crop_x = (cols / 2) - (crop_size / 2);
	} else if (crop_position == 2) {
		crop_x = cols - crop_size;
	} else if (crop_position == 3) {
		crop_y = (rows / 2) - (crop_size / 2);
	} else if (crop_position == 4) {
		crop_x = (cols / 2) - (crop_size / 2);
		crop_y = (rows / 2) - (crop_size / 2);
	} else if (crop_position == 5) {
		crop_x = cols - crop_size;
		crop_y = (rows / 2) - (crop_size / 2);
	} else if (crop_position == 6) {
		crop_y = rows - crop_size;
	} else if (crop_position == 7) {
		crop_x = (cols / 2) - (crop_size / 2);
		crop_y = rows - crop_size;
	} else if (crop_position == 8) {
		crop_x = cols - crop_size;
		crop_y = rows - crop_size;
	}

	// Crop image
	cv::Rect crop_region = cv::Rect(crop_x, crop_y, crop_size, crop_size); 
	cv::Mat crop_image = src(crop_region);

	// Copy image
	cv::Mat dst = cv::Mat::zeros(rows, cols, src.type());
	int center_x = cols/2 - (crop_size/2);
	int center_y = rows/2 - (crop_size/2);
	cv::Rect roi(center_x, center_y, crop_size, crop_size);
	cv::Mat target_roi = dst(roi);
	crop_image.copyTo(target_roi); 

	std::memcpy(data, dst.data, dlen);
}
