#include "XPT2046.hpp"
#include "stm32l4xx_hal.h"
#include <cstdio>
#include <cstring>

extern UART_HandleTypeDef huart2;

XPT2046::XPT2046(SPI_HandleTypeDef* hspi, XPT2046PinConf cs, XPT2046PinConf irq) :
	hspi_(hspi), cs_(cs), irq_(irq) {}

TouchPoint XPT2046::readCoordinates() {
	uint16_t adcX = 0;
	uint16_t adcY = 0;

	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_, commands, buf_, sizeof(buf_), 1000);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_SET);
	adcX = (buf_[1] << 4) | (buf_[2] >> 4);
	adcY = (buf_[4] << 4) | (buf_[5] >> 4);
	sendData(adcX, adcY);

	TouchPoint points = { .x = adcX, .y = adcY };

	return points;
}

void XPT2046::sendData(uint16_t adcX, uint16_t adcY) {
	char buf[32];
	snprintf(buf, sizeof(buf), "%u,%u\n", adcX, adcY);
	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), 1000);
	HAL_Delay(500);
}

