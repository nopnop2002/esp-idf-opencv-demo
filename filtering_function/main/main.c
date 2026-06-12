/*
	OpenCV Example.

	This code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.

	I ported from here:
	https://github.com/espressif/esp-opencv-component/tree/master/examples/motion_detection

	I used this as a reference:
	https://book.mynavi.jp/files/user/support/pc/opencv2/c3/opencv_img.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"

#include "esp_camera.h"
#include "camera_pin.h"

#include "cmd.h"

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char *TAG = "MAIN";

static int s_retry_num = 0;

QueueHandle_t xQueueCmd;
QueueHandle_t xQueueHttp;

static camera_config_t camera_config = {
	.pin_pwdn  = CAM_PIN_PWDN,
	.pin_reset = CAM_PIN_RESET,
	.pin_xclk = CAM_PIN_XCLK,
	.pin_sscb_sda = CAM_PIN_SIOD,
	.pin_sscb_scl = CAM_PIN_SIOC,

	.pin_d7 = CAM_PIN_D7,
	.pin_d6 = CAM_PIN_D6,
	.pin_d5 = CAM_PIN_D5,
	.pin_d4 = CAM_PIN_D4,
	.pin_d3 = CAM_PIN_D3,
	.pin_d2 = CAM_PIN_D2,
	.pin_d1 = CAM_PIN_D1,
	.pin_d0 = CAM_PIN_D0,
	.pin_vsync = CAM_PIN_VSYNC,
	.pin_href = CAM_PIN_HREF,
	.pin_pclk = CAM_PIN_PCLK,

	//XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
	.xclk_freq_hz = 20000000,
	.ledc_timer = LEDC_TIMER_0,
	.ledc_channel = LEDC_CHANNEL_0,

	//OpenCV can only read monochrome images.
	.pixel_format = PIXFORMAT_GRAYSCALE,//YUV422,PIXFORMAT_GRAYSCALE,RGB565,PIXFORMAT_JPEG
	.frame_size = FRAMESIZE_HVGA, //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

	.jpeg_quality = 12, //0-63 lower number means higher quality
	.fb_count = 1, //if more than one, i2s runs in continuous mode. Use only with JPEG
	.grab_mode = CAMERA_GRAB_WHEN_EMPTY,
	.fb_location = CAMERA_FB_IN_PSRAM,
};

void cv_print_info();
void cv_vertical_flip(uint8_t* data, int dlen, int width, int height);
void cv_horizontal_flip(uint8_t* data, int dlen, int width, int height);
void cv_flip(uint8_t* data, int dlen, int width, int height);
void cv_invert(uint8_t* data, int dlen, int width, int height);

void cv_vertical_concat(uint8_t* data, int dlen, int width, int height);
void cv_horizontal_concat(uint8_t* data, int dlen, int width, int height);
void cv_concat(uint8_t* data, int dlen, int width, int height);
void cv_zoom(uint8_t* data, int dlen, int width, int height);

void cv_filter_laplacian(uint8_t* data, int dlen, int width, int height);
void cv_filter_canny(uint8_t* data, int dlen, int width, int height);

void cv_filter_thresh_binary(uint8_t* data, int dlen, int width, int height);
void cv_filter_thresh_otsu(uint8_t* data, int dlen, int width, int height);

void cv_filter_filter_2d(uint8_t* data, int dlen, int width, int height, int format, int filter);
void cv_filter_sep_filter_2d(uint8_t* data, int dlen, int width, int height);
void cv_filter_blur(uint8_t* data, int dlen, int width, int height);
void cv_filter_gaussian_blur(uint8_t* data, int dlen, int width, int height);
void cv_filter_median_blur(uint8_t* data, int dlen, int width, int height);
void cv_filter_bilateral(uint8_t* data, int dlen, int width, int height);

void cv_equalize_hist(uint8_t* data, int dlen, int width, int height);
void cv_create_clahe(uint8_t* data, int dlen, int width, int height);
void cv_sobel_differential(uint8_t* data, int dlen, int width, int height);
void cv_erode(uint8_t* data, int dlen, int width, int height);
void cv_dilate(uint8_t* data, int dlen, int width, int height);
void cv_morphology(uint8_t* data, int dlen, int width, int height);

esp_err_t camera_capture(char * filterName, char * sourceFileName, char * opencvFileName)
{
	size_t jpg_buf_len = 0;
	uint8_t * jpg_buf = NULL;

	// Clear internal queue
	//for(int i=0;i<2;i++) {
	for(int i=0;i<1;i++) {
		camera_fb_t * fb = esp_camera_fb_get();
		ESP_LOGI(TAG, "fb->len=%d", fb->len);
		esp_camera_fb_return(fb);
	}

	// Acquire a frame
	camera_fb_t * fb = esp_camera_fb_get();
	if (!fb) {
		ESP_LOGE(TAG, "Camera Capture Failed");
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "fb->len=%d", fb->len);

	// Convert camera frame buffer to JPEG
	// jpg_buf needs to be released
	bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
	if(!jpeg_converted){
		ESP_LOGE(TAG, "JPEG compression failed");
		esp_camera_fb_return(fb);
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "jpg_buf_len=%d", jpg_buf_len);

	// JPEG before filtering
	FILE* fs = fopen(sourceFileName, "wb");
	if (fs == NULL) {
		ESP_LOGE(TAG, "Failed to open source file for writing");
		free(jpg_buf);
		return ESP_FAIL; 
	}
	fwrite(jpg_buf, jpg_buf_len, 1, fs);
	fclose(fs);
	free(jpg_buf);

	// OpenCV process
	if (strcmp(filterName, "VerticalFlip") == 0) {
		cv_vertical_flip(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "HorizontalFlip") == 0) {
		cv_horizontal_flip(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Flip") == 0) {
		cv_flip(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "Invert") == 0) {
		cv_invert(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "VerticalConcat") == 0) {
		cv_vertical_concat(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "HorizontalConcat") == 0) {
		cv_horizontal_concat(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "Concat") == 0) {
		cv_concat(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "Zoom") == 0) {
		cv_zoom(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Laplacian") == 0) {
		cv_filter_laplacian(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Canny") == 0) {
		cv_filter_canny(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "ThreshBinary") == 0) {
		cv_filter_thresh_binary(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "ThreshOtsu") == 0) {
		cv_filter_thresh_otsu(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "Filter2D_HighPass") == 0) {
		cv_filter_filter_2d(fb->buf, fb->len, fb->width, fb->height, fb->format, 0);  
	} else if (strcmp(filterName, "Filter2D_Gradient") == 0) {
		cv_filter_filter_2d(fb->buf, fb->len, fb->width, fb->height, fb->format, 1);  
	} else if (strcmp(filterName, "Filter2D_Laplacian") == 0) {
		cv_filter_filter_2d(fb->buf, fb->len, fb->width, fb->height, fb->format, 2);  
	} else if (strcmp(filterName, "Filter2D_Gaussian") == 0) {
		cv_filter_filter_2d(fb->buf, fb->len, fb->width, fb->height, fb->format, 3);  
	} else if (strcmp(filterName, "Filter2D_Embossed") == 0) {
		cv_filter_filter_2d(fb->buf, fb->len, fb->width, fb->height, fb->format, 4);  
	} else if (strcmp(filterName, "SepFilter2D") == 0) {
		cv_filter_sep_filter_2d(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Blur") == 0) {
		cv_filter_blur(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "GaussianBlur") == 0) {
		cv_filter_gaussian_blur(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "MedianBlur") == 0) {
		cv_filter_median_blur(fb->buf, fb->len, fb->width, fb->height);	
	} else if (strcmp(filterName, "BilateralFilter") == 0) {
		cv_filter_bilateral(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "EqualizeHist") == 0) {
		cv_equalize_hist(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "CreateCLAHE") == 0) {
		cv_create_clahe(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "SobelDifferentiation") == 0) {
		cv_sobel_differential(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Erode") == 0) {
		cv_erode(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Dilate") == 0) {
		cv_dilate(fb->buf, fb->len, fb->width, fb->height);  
	} else if (strcmp(filterName, "Morphology") == 0) {
		cv_morphology(fb->buf, fb->len, fb->width, fb->height);  
	} else {
		ESP_LOGE(TAG, "Illegal filter name");
		esp_camera_fb_return(fb);
		return ESP_FAIL;
	}

	// Convert camera frame buffer to JPEG
	// jpg_buf needs to be released
	jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
	if(!jpeg_converted){
		ESP_LOGE(TAG, "JPEG compression failed");
		esp_camera_fb_return(fb);
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "jpg_buf_len=%d", jpg_buf_len);

	// JPEG after filtering
	FILE* fo = fopen(opencvFileName, "wb");
	if (fo == NULL) {
		ESP_LOGE(TAG, "Failed to open opencv file for writing");
		free(jpg_buf);
		esp_camera_fb_return(fb);
		return ESP_FAIL; 
	}
	fwrite(jpg_buf, jpg_buf_len, 1, fo);
	fclose(fo);
	free(jpg_buf);

	// return the frame buffer back to the driver for reuse
	esp_camera_fb_return(fb);

	return ESP_OK;
}

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(TAG,"connect to the AP fail");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

esp_err_t wifi_init_sta()
{
	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
		ESP_EVENT_ANY_ID,
		&event_handler,
		NULL,
		&instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
		IP_EVENT_STA_GOT_IP,
		&event_handler,
		NULL,
		&instance_got_ip));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = CONFIG_ESP_WIFI_SSID,
			.password = CONFIG_ESP_WIFI_PASSWORD
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
	 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
esp_err_t ret_value = ESP_OK;
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
		WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
		pdFALSE,
		pdFALSE,
		portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
	 * happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
		ret_value = ESP_FAIL;
	} else {
		ESP_LOGE(TAG, "UNEXPECTED EVENT");
		ret_value = ESP_FAIL;
	}

	/* The event will not be processed after unregister */
	ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
	ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
	vEventGroupDelete(s_wifi_event_group);
	return ret_value;
}


static void listSPIFFS(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		if (pe->d_type == 1) {
			char fullPath[64];
			strcpy(fullPath, path);
			strcat(fullPath, "/");
			strcat(fullPath, pe->d_name);
			struct stat st;
			stat(fullPath, &st);
			ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x size=%d", 
				pe->d_name, pe->d_ino, pe->d_type, st.st_size);
		}
	}
	closedir(dir);
}

esp_err_t mountSPIFFS(char * path, char * label, int max_files) {
	esp_vfs_spiffs_conf_t conf = {
		.base_path = path,
		.partition_label = label,
		.max_files = max_files,
		.format_if_mount_failed = true
	};

	// Use settings defined above to initialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret ==ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret== ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return ret;
	}

#if 0
	ESP_LOGI(TAG, "Performing SPIFFS_check().");
	ret = esp_spiffs_check(conf.partition_label);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
		return ret;
	} else {
			ESP_LOGI(TAG, "SPIFFS_check() successful");
	}
#endif

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(conf.partition_label, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG,"Mount %s to %s success", path, label);
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
	}

	return ret;
}

void keyin(void *pvParameters);
void http_task(void *pvParameters);

void app_main(void)
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initilize WiFi
	ESP_ERROR_CHECK(wifi_init_sta());

	// Mount SPIFFS
	char *base_path = "/spiffs";
	ESP_ERROR_CHECK(mountSPIFFS(base_path, "storage", 2));
	listSPIFFS("/spiffs/");

#if CONFIG_ENABLE_FLASH
	// Setup Flash Light
	gpio_reset_pin(CONFIG_GPIO_FLASH);
	gpio_set_direction(CONFIG_GPIO_FLASH, GPIO_MODE_OUTPUT);
	gpio_set_level(CONFIG_GPIO_FLASH, 0);
#endif

	// Create Queue
	xQueueCmd = xQueueCreate( 1, sizeof(CMD_t) );
	configASSERT( xQueueCmd );
	xQueueHttp = xQueueCreate( 10, sizeof(HTTP_t) );
	configASSERT( xQueueHttp );

	// Function table
	uint16_t commands[] = 
		{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	char *functions[] = {
		"VerticalFlip", // a
		"HorizontalFlip", // b
		"Flip", // c
		"Invert", // d
		"VerticalConcat", // e
		"HorizontalConcat", // f
		"Concat", // g
		"Laplacian", // h
		"Canny", // i
		"ThreshBinary", // j
		"ThreshOtsu", // k
		"Filter2D_HighPass", // l
		"Filter2D_Gradient", // m
		"Filter2D_Laplacian", // n
		"Filter2D_Gaussian", // o
		"Filter2D_Embossed", // p
		"SepFilter2D", // q
		"GaussianBlur", // r
		"MedianBlur", // s
		"BilateralFilter", // t
		"EqualizeHist", // u
		"CreateCLAHE", // v
		"SobelDifferentiation", // w
		"Erode", // x
		"Dilate", // y
		"Morphology" // z
	};

	// Get the local IP address
	esp_netif_ip_info_t ip_info;
	ESP_ERROR_CHECK(esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info));

	// Create HTTP Task
	char cparam0[64];
	sprintf(cparam0, IPSTR, IP2STR(&ip_info.ip));
	ESP_LOGI(TAG, "cparam0=[%s]", cparam0);
	xTaskCreate(http_task, "HTTP", 1024*6, (void *)cparam0, 2, NULL);

	// Create trigger Task
#define SHUTTER "Keybord Enter"
	xTaskCreate(keyin, "KEYIN", 1024*4, NULL, 2, NULL);
	
	// OpenCV info print
	cv_print_info();

	// Initialize camera
	ret = esp_camera_init(&camera_config);
	if (ret != ESP_OK) {
		ESP_LOGE("CAM", "Camera Init Failed");
		return;
	}
	sensor_t *s = esp_camera_sensor_get();
	ESP_LOGI("CAM", "Camera sensor %2.2x %2.2x %4.4x %2.2x", s->id.MIDH, s->id.MIDL, s->id.PID, s->id.VER);

	//initial sensors are flipped vertically and colors are a bit saturated
	if (s->id.PID == OV3660_PID)
	{
		s->set_brightness(s, 1);  //up the brightness just a bit
		s->set_saturation(s, -2); //lower the saturation
	}

	HTTP_t httpBuf;
	sprintf(httpBuf.sourceFileName, "%s/source.jpg", base_path);
	ESP_LOGI(TAG, "sourceFileName=%s",httpBuf.sourceFileName);
	sprintf(httpBuf.opencvFileName, "%s/opencv.jpg", base_path);
	ESP_LOGI(TAG, "opencvFileName=%s",httpBuf.opencvFileName);

	CMD_t cmdBuf;
	while(1) {
		int commandNum= sizeof(commands)/2;
		for (int i=0;i<commandNum;i++) {
			printf("%c:%s\n", commands[i], functions[i]);
		}
		ESP_LOGD(TAG,"Waitting %s ....", SHUTTER);
		xQueueReceive(xQueueCmd, &cmdBuf, portMAX_DELAY);
		ESP_LOGI(TAG,"cmdBuf.command=%x[%c]", cmdBuf.command, cmdBuf.command);

#if CONFIG_ENABLE_FLASH
		// Flash Light ON
		gpio_set_level(CONFIG_GPIO_FLASH, 1);
#endif
		memset(httpBuf.filterName, 0, sizeof(httpBuf.filterName));
		for (int i=0;i<commandNum;i++) {
			ESP_LOGD(TAG, "commands[i]=%x", commands[i]);
			if (cmdBuf.command == commands[i]) strcpy(httpBuf.filterName, functions[i]);
		}
		ESP_LOGI(TAG, "httpBuf.filterName=[%s]", httpBuf.filterName);
		if (strlen(httpBuf.filterName) == 0) continue;

		while(1) {
			struct stat stat1;
			if (stat(httpBuf.sourceFileName, &stat1) == 0) {
				// Delete it if it exists
				unlink(httpBuf.sourceFileName);
			}
			struct stat stat2;
			if (stat(httpBuf.opencvFileName, &stat2) == 0) {
				// Delete it if it exists
				unlink(httpBuf.opencvFileName);
			}
			camera_capture(httpBuf.filterName, httpBuf.sourceFileName, httpBuf.opencvFileName);
			//listSPIFFS("/spiffs/");
			if (stat(httpBuf.sourceFileName, &stat1) == 0) {
				if (stat(httpBuf.opencvFileName, &stat2) == 0) {
					ESP_LOGI(TAG, "stat1.st_size=%d stat2.st_size=%d", stat1.st_size, stat2.st_size);
					if (stat1.st_size != 0 && stat2.st_size != 0) break;
				}
			}
			vTaskDelay(100);
		}

#if CONFIG_ENABLE_FLASH
		// Flash Light OFF
		gpio_set_level(CONFIG_GPIO_FLASH, 0);
#endif

		// send image file name to http task
		if (xQueueSend(xQueueHttp, &httpBuf, 10) != pdPASS) {
			ESP_LOGE(TAG, "xQueueSend xQueueHttp fail");
		}
	} // end while
}
