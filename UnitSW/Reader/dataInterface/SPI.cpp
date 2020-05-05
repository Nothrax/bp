//
// Created by Jakub Trubka on 21.10.19.
//

#include "SPI.h"
//#include ""

void SPI::start() {
    SHMRawWrite *buffer = new SHMRawWrite(config);
    buffer->openSharedMemory();
    bcm2835_gpio_set_eds(config.dataReadyPin);
    bcm2835_gpio_afen(config.dataReadyPin);

    while(true){
        bcm2835_gpio_set_eds(config.dataReadyPin);//clear the previous event. makes sure that only next event is detected
        while(!bcm2835_gpio_eds(config.dataReadyPin));//waits until next event/tick
        bcm2835_spi_transfernb((char *)rreg_tx, (char *)default_rx, sizeof(rreg_tx));

        buffer->addMeasurement(default_rx) ;
    }
    bcm2835_gpio_set_eds(config.dataReadyPin);
    bcm2835_gpio_clr_afen(config.dataReadyPin);
}


void SPI::setUp() {
    printf("Inicializace\n");
    initializeSPI();
    initializeODPins();
    restartAdc();
    initializeAdc();
}

void SPI::setRegisterSafe(unsigned char *transferRegister, unsigned char *receiveRegister, std::string setupPart) {
    uint8_t first;
    uint8_t second;
    int count = 0;

    do {
        uint8_t received [ARRAY_SIZE(default_tx)] = {0, };
        bcm2835_spi_transfernb((char *)transferRegister, (char *)default_rx, sizeof(default_tx));
        bcm2835_spi_transfernb((char *)default_tx, (char *)received, sizeof(default_tx));
        first = received[0];
        second = received[1];
    } while(first != receiveRegister[0] && second != receiveRegister[1] && ++count < 10);
    if(count == 10) {
        Logger::logError("Setup error " + setupPart);
        exit(EXIT_FAILURE);
    };
}


SPI::SPI(Config config) {
    this->config = config;
    if(config.frequency != 8000 && config.frequency != 64000 && config.frequency != 128000){
        Logger::logError("Frequency should be 8/64/128 kHz, terminating reader");
        exit(EXIT_FAILURE);
    }
}

void SPI::initializeSPI() {
    fd = open("/dev/spidev0.0", O_RDWR);

    if (fd < 0) {
        Logger::logError("Can't open device");
        exit(EXIT_FAILURE);
    }

    if (!bcm2835_init())  {
        Logger::logError("Bcm2835_init failed. Are you running as root?");
        exit(EXIT_FAILURE);
    }
    if (!bcm2835_spi_begin()) {
        Logger::logError("bcm2835_spi_begin failed. Are you running as root?\n");
        exit(EXIT_FAILURE);
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      	// The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   	// The default
    bcm2835_spi_set_speed_hz(config.spiSpeed);

}

void SPI::restartAdc() {
    bcm2835_gpio_fsel(config.restartAdcPin,BCM2835_GPIO_FSEL_OUTP); //old physical 37 (26)
    bcm2835_gpio_set(config.restartAdcPin);

    usleep(5000);
    bcm2835_gpio_clr(config.restartAdcPin);
    usleep(5000);
}

SPI::~SPI() {
    bcm2835_gpio_set_eds(config.dataReadyPin);
    bcm2835_gpio_clr_afen(config.dataReadyPin);
    bcm2835_spi_end();
}

void SPI::initializeAdc() {
    bcm2835_spi_transfernb((char *)statp_tx,(char *)default_rx,sizeof(default_tx));
    bcm2835_spi_transfernb((char *)default_tx,(char *)default_rx,sizeof(default_tx));
    bcm2835_spi_transfernb((char *)statn_tx,(char *)default_rx,sizeof(default_tx));

    bcm2835_spi_transfernb((char *)default_tx,(char *)default_rx,sizeof(default_tx));
    bcm2835_spi_transfernb((char *)reset_tx,(char *)reset_rx,sizeof(default_tx));
    bcm2835_spi_transfernb((char *)default_tx,(char *)default_rx,sizeof(default_tx));

    setRegisterSafe(unlock_tx, unlock_rx, "Unlock device");
    setRegisterSafe(crc_tx, crc_rx, "Set register D_SYS_CFG");
    setRegisterSafe(clk1_tx, clk1_rx, "Set register CLK1");

    //todo set frequency by config
    setRegisterSafe(clk_128k_tx, clk_128k_rx, "Set register CLK2\n");

    setRegisterSafe(sysconf_tx, sysconf_rx, "Set register A_SYS_CFG");
    setRegisterSafe(adc_ena_tx, adc_ena_rx, "ADC enable\n");
    setRegisterSafe(wake_up_tx, wake_up_rx, "Wakeup device\n");
    setRegisterSafe(lock_com_tx, lock_com_rx, "Lock registers\n");
}

void SPI::initializeODPins() {
    //setting output disable pins
    for(int i = 0; i < NUMBER_OF_SENSORS; i++){
        bcm2835_gpio_fsel(config.outputDisablePin[i],BCM2835_GPIO_FSEL_OUTP);
        if(config.sensorActive[i]) {
            bcm2835_gpio_set(config.outputDisablePin[i]);
            //printf("Setting pin %d, channel %d to HIGH\n",config.outputDisablePin[i], i);
        } else {
            bcm2835_gpio_clr(config.outputDisablePin[i]);
        }
    }
}

uint16_t SPI::checkCRC(uint8_t *ptr, int count) {
    int  crc;
    char i;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return (crc);
}