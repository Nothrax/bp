//
// Created by Jakub Trubka on 21.10.19.
//

#include "File.h"

void File::start() {
    SHMRawWrite *buffer = new SHMRawWrite(config);
    buffer->openSharedMemory();

    bcm2835_gpio_set_eds(22);//clear the previous event. makes sure that only next event is detected

    while(true){
        bcm2835_gpio_set_eds(config.dataReadyPin);//clear the previous event. makes sure that only next event is detected
        while(!bcm2835_gpio_eds(config.dataReadyPin));//waits until next event/tick

        input.read((char *)&default_rx[3], 12);
        buffer->addMeasurement(default_rx) ;
    }
    bcm2835_gpio_set_eds(config.dataReadyPin);
    bcm2835_gpio_clr_afen(config.dataReadyPin);

}

void File::setUp() {
    input.open(config.dataFilePath);
    gpioInitialise();
    if(gpioHardwareClock(4, config.frequency) < 0) {
        Logger::logError("Failed to set output data ready frequency");
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
