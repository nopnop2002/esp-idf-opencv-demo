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

extern "C" void cv_shape_detection(uint8_t* data, int dlen, int width, int heigth);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

void cv_shape_detection(uint8_t* data, int dlen, int width, int heigth)
{
	printf("OpenCV shape detection.\n");
	cv::Mat src( heigth, width, CV_8UC1, (void*)data );
	cv::Mat cloned = src.clone();
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	// Specify the characters to insert, their position, and font parameters.
	std::string text = "OpenCV C++ Text!";
	//cv::Point org(0, 0); // The lower left coordinate of the character
	int fontFace = cv::FONT_HERSHEY_SIMPLEX; // Font style
	//double fontScale = 1.0; // Font size magnification
	double fontScale = 0.5; // Font size magnification
	cv::Scalar color(0, 255, 0); // Font color
	int thickness = 1; // Line thickness
	int lineType = cv::LINE_8; // Line type

	// Edge detection
	cv::Mat edges;
	cv::Canny(src, edges, 100, 200);
	//cv::threshold(src, edges, 127, 255, cv::THRESH_BINARY);

	// Contour extraction
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(edges, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	std::vector<cv::Point> approx;

	for (size_t i = 0; i < contours.size(); i++) {
		// Approximating the contour (reducing the number of polygon vertices)
		double epsilon = 0.02 * cv::arcLength(contours[i], true);
		cv::approxPolyDP(contours[i], approx, epsilon, true);

		// Outline conditions
		// Condition 1: There are N vertices.
		// Condition 2: It is a closed figure.
		// Condition 3: There is a certain amount of area.
		if (cv::isContourConvex(approx) && cv::contourArea(approx) > 1000) {
			if (approx.size() == 3) {
				std::cout << "found: P1 =" << approx[0] << ", P2 =" << approx[1] << ", P3 =" << approx[2] << std::endl;
				for (int j = 0; j < approx.size(); j++) {
					cv::line(cloned, approx[j], approx[(j + 1) % approx.size()], cv::Scalar(0, 255, 0), 2);
				}
				text = "triangle";
				cv::putText(cloned, text, approx[0], fontFace, fontScale, color, thickness, lineType);
			} else if (approx.size() == 4) {
				std::cout << "found: P1 =" << approx[0] << ", P2 =" << approx[1] << ", P3 =" << approx[2] << ", P4 =" << approx[3] << std::endl;
				for (int j = 0; j < approx.size(); j++) {
					cv::line(cloned, approx[j], approx[(j + 1) % approx.size()], cv::Scalar(0, 255, 0), 2);
				}
				text = "rectangle";
				cv::putText(cloned, text, approx[0], fontFace, fontScale, color, thickness, lineType);
			} else if (approx.size() == 5) {
				std::cout << "found: P1 =" << approx[0] << ", P2 =" << approx[1] << ", P3 =" << approx[2] << ", P4 =" << approx[3] << std::endl;
				for (int j = 0; j < approx.size(); j++) {
					cv::line(cloned, approx[j], approx[(j + 1) % approx.size()], cv::Scalar(0, 255, 0), 2);
				}
				text = "pentagon";
				cv::putText(cloned, text, approx[0], fontFace, fontScale, color, thickness, lineType);
			} else if (approx.size() == 6) {
				std::cout << "found: P1 =" << approx[0] << ", P2 =" << approx[1] << ", P3 =" << approx[2] << ", P4 =" << approx[3] << std::endl;
				for (int j = 0; j < approx.size(); j++) {
					cv::line(cloned, approx[j], approx[(j + 1) % approx.size()], cv::Scalar(0, 255, 0), 2);
				}
				text = "hexagon";
				cv::putText(cloned, text, approx[0], fontFace, fontScale, color, thickness, lineType);
			}
		}
	} // end for

	// Noise removal
	cv::Mat blurred;
	cv::medianBlur(src, blurred, 5);

	// Circle detection using Hough transform
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(
		blurred, 
		circles, 
		cv::HOUGH_GRADIENT, 
		1,   // Ratio of accumulator resolution to image resolution
		20,  // Minimum distance between the centers of detected circles
		100, // Upper limit of the threshold for the Canny edge detector
		30,  // Accumulator threshold
		20,  // Minimum radius of the circle to detect (in pixels)
		100  // Maximum radius of the circle to detect (in pixels)
	);

	for (size_t i = 0; i < circles.size(); i++) {
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		std::cout << "found: C1 =" << circles[i][0] << ", C2 =" << circles[i][1] << std::endl;
		
		// Draw the center of the circle
		cv::circle(cloned, center, 3, cv::Scalar(0, 255, 0), -1, lineType, 0);
		// Draw the outline of a circle
		cv::circle(cloned, center, radius, cv::Scalar(0, 0, 255), 2, lineType, 0);
		// Draw the text
		text = "circle";
		cv::putText(cloned, text, center, fontFace, fontScale, color, thickness, lineType);
	}

	printf("========================\n\n");
	std::memcpy(data, cloned.data, dlen);
}
