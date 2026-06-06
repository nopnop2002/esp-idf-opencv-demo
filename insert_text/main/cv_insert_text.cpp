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

extern "C" void cv_insert_text(uint8_t* data, int dlen, int width, int heigth, uint16_t color);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

#define RGB888_TO_RGB565(r, g, b) ( \
    (((r) & 0xF8) << 8) | \
    (((g) & 0xFC) << 3) | \
    ((b) >> 3) \
)

#define RGB888_TO_16UC1(b, r, g) ( \
    (((b) & 0xF8) << 8) | \
    (((r) & 0xFC) << 3) | \
    ((g) >> 3) \
)

void cv_insert_text(uint8_t* data, int dlen, int width, int heigth, uint16_t color)
{
	printf("OpenCV insert text.\n");
	//cv::Mat src( heigth, width, CV_8UC1, (void*)data );
	//cv::Mat src( heigth, width, CV_8UC2, (void*)data );
	cv::Mat src( heigth, width, CV_16UC1, (void*)data );
	cv::Mat cloned = src.clone();
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	// Specify the characters to insert, their position, and font parameters.
    std::string text = "OpenCV C++ Text";
    cv::Point org(width/2, heigth/2); // The lower left coordinate of the character
    int fontFace = cv::FONT_HERSHEY_SIMPLEX; // Font style
    double fontScale = 0.5; // Font size magnification
    //cv::Scalar color(0, 0, 0); // Font color
    int thickness = 1; // Line thickness
    int lineType = cv::LINE_8; // Line type

	//uint16_t color=RGB888_TO_16UC1(255, 0, 0); // Blue
	//uint16_t color=RGB888_TO_16UC1(0, 255, 0); // Red
	//uint16_t color=RGB888_TO_16UC1(0, 0, 255); // Green
	//uint16_t color=RGB888_TO_16UC1(0, 255, 255); // Yellow
	//uint16_t color=RGB888_TO_16UC1(255, 0, 255); // Cyan
	//uint16_t color=RGB888_TO_16UC1(255, 255, 0); // Purple
	//printf("color=0x%x\n", color);

	// Insert text into an image
    cv::putText(cloned, text, org, fontFace, fontScale, color, thickness, lineType);

	std::memcpy(data, cloned.data, dlen);
}
