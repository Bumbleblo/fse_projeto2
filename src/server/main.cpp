#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <string.h>

#include "socket.hpp"
#include "bme280.h"
#include "gpio.hpp"
#include "linux_userspace.h"

#define PORT 8000

char *centralIp;

void setupBME280(struct bme280_dev *dev, struct identifier *id, char *path){
    configure_bme280(dev, id, path);
    int code = bme280_init(dev);

    if(code!=BME280_OK){
        std::cerr << "Failed to initialize BME280" << std::endl;
        exit(1);
    }
}



int main(int argc, char **argv){


    // bme280
    struct bme280_data sensor_data;
    struct bme280_dev bme280_device;
    struct identifier id;

    setupBME280(&bme280_device, &id, "/dev/i2c-1");

    std::cout << argc << std::endl;

    if(argc != 2){

        std::cerr << "Send IP" << std::endl;
        exit(9);
    }

    centralIp = argv[1];
    std::cout << centralIp << std::endl;

    // threads

    char *rbuffer, sbuffer[100];
    char command[20];
    int size;
    int index, value;

    fsesocket::SocketClient msg_socket = fsesocket::SocketClient(centralIp, PORT);

    gpio::setupGPIO(&msg_socket);
    while(true){

        // Sending sensor data
        sensor_data = readTemperatureData(&bme280_device);
        sprintf(sbuffer, "SENSORDATA=%.2lf:%.2lf$\0", sensor_data.temperature, sensor_data.humidity);
        msg_socket.sendMessage(sbuffer, 50);

        // Receive message
        rbuffer = msg_socket.receiveMessage(50, &size);


        if(size > 0){
            rbuffer[49]= '\0';
            std::cout << rbuffer << std::endl;

            sscanf(rbuffer, "%s %d %d", command, &index, &value);

            std::cout << command << " " << index << " " << value << std::endl;

            gpio::handleCommand(command, index, value);
            free(rbuffer);
            rbuffer = NULL;
        }


        // sleep time
        sleep(1);

    }

    msg_socket.closeSocket();

}
