#include "app.hpp"
#include "ILI9341.hpp"
#include "sample.hpp"
#include "sample2.hpp"

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
	ili9341.fillScreen(0x0000);
	ili9341.drawLine(0, 0, 239, 319, 0xF800);    // діагональ
	ili9341.drawLine(0, 160, 239, 160, 0x07E0);  // горизонтальна
	ili9341.drawLine(120, 0, 120, 319, 0x001F);  // вертикальна
	ili9341.drawRect(50, 50, 50, 100, 0x0008FF);
	ili9341.drawCircle(120, 160, 50, 0x07FF);
	ili9341.drawChar(10, 10, '!', 0x07FF);
	ili9341.drawChar(200, 200, 'A', 0x07FF);
	ili9341.drawString(10, 200, "Pavlo Hello", 0x001F);
	ili9341.drawString(10, 220, "Happy Current Year!", 0x001F);
	ili9341.drawString(10, 240, "Disconcerted", 0x001F);
//	ili9341.drawImage(sample_data, sample_width, sample_height);
	ili9341.drawImage(sample2_data, sample2_width, sample2_height);
}
