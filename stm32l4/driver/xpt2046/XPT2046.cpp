#include "XPT2046.hpp"

XPT2046::XPT2046(SPI_HandleTypeDef* hspi, XPT2046PinConf cs, XPT2046PinConf irq) :
	hspi_(hspi), cs_(cs), irq_(irq) {}

TouchPoint XPT2046::readCoordinates() {
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi_, commands, buf_, sizeof(buf_), 1000);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_SET);
	uint16_t adcX = (buf_[1] << 4) | (buf_[2] >> 4);
	uint16_t adcY = (buf_[4] << 4) | (buf_[5] >> 4);
	TouchPoint points = { .x = adcX, .y = adcY };

	return points;
}

