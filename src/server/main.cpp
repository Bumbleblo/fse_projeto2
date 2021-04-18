#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <string.h>

#include "socket.hpp"
#include "bme280.h"
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
    int size;

    fsesocket::SocketClient msg_socket = fsesocket::SocketClient(centralIp, PORT);

    while(true){

        // Sending sensor data
        sensor_data = readTemperatureData(&bme280_device);
        sprintf(sbuffer, "SENSORDATA=%.2lf:%.2lf\0", sensor_data.temperature, sensor_data.humidity);
        msg_socket.sendMessage(sbuffer, strlen(sbuffer));

        // Receive message
        rbuffer = msg_socket.receiveMessage(100, &size);

        rbuffer[99]= '\0';
        std::cout << rbuffer << std::endl;

        free(rbuffer);
        rbuffer = NULL;

        // sleep time
        sleep(1);

    }

    msg_socket.closeSocket();

}
