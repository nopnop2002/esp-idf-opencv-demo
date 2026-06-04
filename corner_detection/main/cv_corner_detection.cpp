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

extern "C" void cv_corner_detection(uint8_t* data, int dlen, int width, int heigth);

extern "C" void cv_print_info();
void cv_print_info()
{
    printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

void cv_corner_detection(uint8_t* data, int dlen, int width, int heigth)
{
    printf("OpenCV corner detection.\n");
    cv::Mat src( heigth, width, CV_8UC1, (void*)data );
	cv::Mat cloned = src.clone();
    int cols = src.cols;
    int rows = src.rows;
    int channels = src.channels();
    int elemSize = src.elemSize();
    printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	// Feature point (angle) detection parameters
    vector<Point2f> corners;
    double maxCorners = 100;      // Maximum number of corners to detect
    double qualityLevel = 0.1;    // Minimum acceptable quality (0.01 or higher recommended)
    double minDist = 20.0;        // Minimum distance between corners (pixels)
    int blockSize = 3;            // neighborhood size

	// Angle detection using the Shi-Tomasi method
	goodFeaturesToTrack(src, corners, maxCorners, qualityLevel, minDist, Mat(), blockSize);
    for (size_t i = 0; i < corners.size(); i++) {
        circle(cloned, corners[i], 5, Scalar(0, 0, 255), -1);
		std::cout << "found: Corner =" << corners[i] << std::endl;
    }
    printf("========================\n\n");
	std::memcpy(data, cloned.data, dlen);
}
