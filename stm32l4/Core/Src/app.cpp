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
ILI9341PinConf csili9341 = {
		.pin = GPIO_PIN_10,
		.port = GPIOA
};
XPT2046PinConf csxp2046 = {
		.pin = GPIO_PIN_0,
		.port = GPIOB
};
XPT2046PinConf irq = {
		.pin = GPIO_PIN_1,
		.port = GPIOB
};
ILI9341 ili9341{&hspi1, dc, reset, csili9341, csxp2046, irq};

float scale = 2.0f;

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
	ili9341.drawImage(sample_data, sample_width, sample_height);
	ili9341.drawImage(sample2_data, sample2_width, sample2_height);
	ili9341.drawMandelbrot(0.5f, 0.0f, 2.0f);
}

void appLoop() {
	scale *= 0.95f;
	if (scale < 0.0001f) scale = 2.0f;
	ili9341.drawMandelbrot(-1.749f, 0.0f, scale);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	ili9341.onDmaDone();
}
