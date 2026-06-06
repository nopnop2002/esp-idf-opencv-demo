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

extern "C" void cv_overlay_image(uint8_t* data, int dlen, int width, int heigth,
	uint8_t* overlay_data, int overlay_width, int overlay_heigth, int overlay_position);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

void cv_overlay_image(uint8_t* data, int dlen, int width, int heigth,
	uint8_t* overlay_data, int overlay_width, int overlay_heigth, int overlay_position)
{
	printf("OpenCV overlap image.\n");
	cv::Mat src( heigth, width, CV_16UC1, (void*)data );
	cv::Mat cloned = src.clone();
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat overlay( overlay_width, overlay_heigth, CV_16UC1, (void*)overlay_data );
	cols = overlay.cols;
	rows = overlay.rows;
	channels = overlay.channels();
	elemSize = overlay.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	// Overlap position(Top left X coordinate, Y coordinate)
	int xpos = 0;
	int ypos = 0;
	if (overlay_position == 1) {
		xpos = (src.cols / 2) - (overlay.cols / 2);
	} else if (overlay_position == 2) {
		xpos = src.cols - overlay.cols;
	} else if (overlay_position == 3) {
		ypos = (src.rows / 2) - (overlay.rows / 2);
	} else if (overlay_position == 4) {
		xpos = (src.cols / 2) - (overlay.cols / 2);
		ypos = (src.rows / 2) - (overlay.rows / 2);
	} else if (overlay_position == 5) {
		xpos = src.cols - overlay.cols;
		ypos = (src.rows / 2) - (overlay.rows / 2);
	} else if (overlay_position == 6) {
		ypos = src.rows - overlay.rows;
	} else if (overlay_position == 7) {
		xpos = (src.cols / 2) - (overlay.cols / 2);
		ypos = src.rows - overlay.rows;
	} else if (overlay_position == 8) {
		xpos = src.cols - overlay.cols;
		ypos = src.rows - overlay.rows;
	}

	// Extract an area from the background image that is the same size as the overlay image
	cv::Rect roi(xpos, ypos, overlay.cols, overlay.rows);
	cv::Mat target_roi = cloned(roi);

	// Overlay an image onto a background image
	overlay.copyTo(target_roi); 

	std::memcpy(data, cloned.data, dlen);
}
