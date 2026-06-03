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

extern "C" void cv_line_detection(uint8_t* data, int dlen, int width, int heigth);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

void cv_line_detection(uint8_t* data, int dlen, int width, int heigth)
{
	printf("OpenCV line detection.\n");
	cv::Mat gray( heigth, width, CV_8UC1, (void*)data );
	cv::Mat cloned = gray.clone();
	int cols = gray.cols;
	int rows = gray.rows;
	int channels = gray.channels();
	int elemSize = gray.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	// Edge detection (Canny method)
	cv::Mat edges;
	cv::Canny(gray, edges, 50, 150);

	// Line detection using stochastic Hough transform
	std::vector<cv::Vec4i> lines;
	//cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);
	cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 100, 50, 50);

	for (size_t i = 0; i < lines.size(); i++) {
		cv::Vec4i l = lines[i];
		// l[0] = x1, l[1] = y1, l[2] = x2, l[3] = y2
		cv::line(cloned, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
		std::cout << "found: P1 =" << l[0] << ", P2 =" << l[1] << ", P3 =" << l[2] << ", P4 =" << l[3] << std::endl;
	}

	printf("========================\n\n");
	std::memcpy(data, cloned.data, dlen);

}
