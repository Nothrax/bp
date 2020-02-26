//
// Created by Jakub Trubka on 21.10.19.
//

#include "File.h"

void File::start() {
    bcm2835_gpio_set_eds(22);//clear the previous event. makes sure that only next event is detected
    while(!bcm2835_gpio_eds(22));//waits until next event/tick
    //input.read((char *)buffer, size);
}

//todo moznost zmeny frekvence?
void File::setUp() {
    input.open(config.dataFilePath);
    gpioInitialise();
    if(gpioHardwareClock(4, config.frequency) < 0) {
        //todo lognuti chyby
    }
    bcm2835_init();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);

    bcm2835_gpio_set_eds(22);
    bcm2835_gpio_afen(22);
}

File::File(Config config) {
    this->config = config;
}

File::~File() {
    input.close();
    gpioTerminate();
    bcm2835_gpio_set_eds(22);
    bcm2835_gpio_clr_afen(22);
}
