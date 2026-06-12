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

#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <math.h>
#include <fstream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace std;
using namespace cv;

extern "C" void cv_vertical_flip(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_horizontal_flip(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_flip(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_invert(uint8_t* data, int dlen, int width, int height);

extern "C" void cv_vertical_concat(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_horizontal_concat(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_concat(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_zoom(uint8_t* data, int dlen, int width, int height);

extern "C" void cv_filter_laplacian(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_filter_canny(uint8_t* data, int dlen, int width, int height);

extern "C" void cv_filter_thresh_binary(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_filter_thresh_otsu(uint8_t* data, int dlen, int width, int height);

extern "C" void cv_filter_filter_2d(uint8_t* data, int dlen, int width, int height, int format, int filter);
extern "C" void cv_filter_sep_filter_2d(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_filter_blur(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_filter_gaussian_blur(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_filter_median_blur(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_filter_bilateral(uint8_t* data, int dlen, int width, int height);

extern "C" void cv_equalize_hist(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_create_clahe(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_sobel_differential(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_erode(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_dilate(uint8_t* data, int dlen, int width, int height);
extern "C" void cv_morphology(uint8_t* data, int dlen, int width, int height);

extern "C" void cv_print_info();
void cv_print_info()
{
	printf("OpenCV info print: %s", cv::getBuildInformation().c_str());
}

// https://qiita.com/Chi_corp_123/items/d870f6cf50d184f6af0f
// https://shimons-labo.com/cvmat%E3%81%A8unsigned-char%E3%81%AE%E7%9B%B8%E4%BA%92%E5%A4%89%E6%8F%9B/#toc2
void cv_vertical_flip(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::flip(src, dst, 0);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_horizontal_flip(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::flip(src, dst, 1);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_flip(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::flip(src, dst, -1);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_invert(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst = ~src;
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_vertical_concat(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat wk;
	cv::resize(src, wk, cv::Size(), 1.0, 0.5);
	cv::Mat	dst;
	cv::vconcat(wk, wk, dst);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_horizontal_concat(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat wk;
	cv::resize(src, wk, cv::Size(), 0.5, 1.0);
	cv::Mat	dst;
	cv::hconcat(wk, wk, dst);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_concat(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat wk;
	cv::resize(src, wk, cv::Size(), 0.5, 0.5);
	cv::hconcat(wk, wk, wk);
	cv::Mat	dst;
	cv::vconcat(wk, wk, dst);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_zoom(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat wk;
	cv::resize(src, wk, cv::Size(), 2.0, 2.0);
	cv::Rect crop_region = cv::Rect(0, 0, width, height);
	cv::Mat dst = wk(crop_region);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_laplacian(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::Laplacian(src, dst, -1);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_canny(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::Canny(src, dst, 50.0, 150.0);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_thresh_binary(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::threshold(src, dst, 140, 255, cv::THRESH_BINARY);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_thresh_otsu(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::threshold(src, dst, 0, 255, cv::THRESH_OTSU);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_filter_2d(uint8_t* data, int dlen, int width, int height, int format, int filter)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::Mat	kernel;

	// High pass filter
	if (filter == 0) {
		kernel = (cv::Mat_<float>(3, 3) << 
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1);
	}

	// Gradient filter
	if (filter == 1) {
		kernel = (cv::Mat_<float>(3, 3) << 
		 1,  1,  1,
		 0,  0,  0,
		-1, -1, -1);
	}

	// Laplacian filter
	if (filter == 2) {
		kernel = (cv::Mat_<float>(3, 3) << 
		 1,  1,  1,
		 1, -8,  1,
		 1,  1,  1);
	}

	// Gaussian filter
	if (filter == 3) {
		kernel = (cv::Mat_<float>(3, 3) << 
		 1,  2,  1,
		 2,  4,  2,
		 1,  2,  1);
	}

	// Embossed filter
	if (filter == 4) {
		kernel = (cv::Mat_<float>(3, 3) << 
		 1,  0,  0,
		 0,  0,  0,
		 0,  0, -1);
	}

	cv::filter2D(src, dst, -1, kernel);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_sep_filter_2d(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::Mat kernelX = (cv::Mat_<float>(1, 3) << 1/3.0, 1/3.0, 1/3.0);
	cv::Mat kernelY = (cv::Mat_<float>(3, 1) << 1/3.0, 1/3.0, 1/3.0);
	cv::sepFilter2D(src, dst, -1, kernelX, kernelY);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_blur(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::blur(src, dst, cv::Size(7, 7));
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_gaussian_blur(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::GaussianBlur(src, dst, cv::Size(5, 5), 0);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_median_blur(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::medianBlur(src, dst, 11);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

void cv_filter_bilateral(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::bilateralFilter(src, dst, 11, 40, 200);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

// https://www.qoosky.dev/techs/a7fd372c07/
void cv_equalize_hist(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::equalizeHist(src, dst);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

// https://www.qoosky.dev/techs/a7fd372c07/
void cv_create_clahe(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat	dst;
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8,8));
	clahe->apply(src, dst);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

// https://www.qoosky.dev/techs/a7fd372c07/
void cv_sobel_differential(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	// Differentiation in the x-axis and y-axis directions
    cv::Mat grad_x, grad_y;
    cv::Sobel(src, grad_x, CV_16S, 1, 0, -1);
    cv::Sobel(src, grad_y, CV_16S, 0, 1, -1);

	// Convert to an unsigned 8-bit integer
    cv::Mat abs_grad_x, abs_grad_y;
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

	cv::Mat	dst;
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

// https://www.qoosky.dev/techs/a7fd372c07/
void cv_erode(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat	dst;
	cv::erode(src, dst, kernel);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

// https://www.qoosky.dev/techs/a7fd372c07/
void cv_dilate(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat	dst;
	cv::dilate(src, dst, kernel);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}

// https://www.qoosky.dev/techs/a7fd372c07/
void cv_morphology(uint8_t* data, int dlen, int width, int height)
{
	cv::Mat src( height, width, CV_8UC1, (void*)data );
	int cols = src.cols;
	int rows = src.rows;
	int channels = src.channels();
	int elemSize = src.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat	dst;
	cv::morphologyEx(src, dst, cv::MORPH_GRADIENT, kernel);
	cols = dst.cols;
	rows = dst.rows;
	channels = dst.channels();
	elemSize = dst.elemSize();
	printf("cols=%d rows=%d channels=%d elemSize=%d\n", cols, rows, channels, elemSize);
	std::memcpy(data, dst.data, dlen);
}
