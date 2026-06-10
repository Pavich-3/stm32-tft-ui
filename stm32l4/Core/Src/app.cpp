#include "app.hpp"
#include "ILI9341.hpp"

extern SPI_HandleTypeDef hspi1;

ILI9341PinConf dc = {
		.pin = GPIO_PIN_9,
		.port = GPIOA
};
ILI9341PinConf reset = {
		.pin = GPIO_PIN_8,
		.port = GPIOA
};
ILI9341PinConf cs = {
		.pin = GPIO_PIN_10,
		.port = GPIOA
};

ILI9341 ili9341{&hspi1, dc, reset, cs};

void appInit() {
	ili9341.init();
	HAL_Delay(500);
	ili9341.fillScreen(0xF800);
}
