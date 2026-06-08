typedef enum {CMD_TAKE, CMD_RESTART, CMD_HALT, CMD_FTP} DIRECTION;

typedef struct {
	uint16_t command;
	TaskHandle_t taskHandle;
} CMD_t;

// Message to HTTP Server
typedef struct {
	char filterName[64];
	char sourceFileName[64];
	char opencvFileName[64];
	TaskHandle_t taskHandle;
} HTTP_t;
