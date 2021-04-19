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

int main(int argc, char **argv){

    // bme280
    struct bme280_data sensor_data;
    struct bme280_dev bme280_device;
    struct identifier id;

    setupBME280(&bme280_device, &id, "/dev/i2c-1");

    if(argc != 2){

        std::cerr << "Usage ./projeto <central ip> " << std::endl;
        exit(9);
    }

    char *centralIp = argv[1];

    std::cout << centralIp << std::endl;

    // socket buffers and command variables
    char *rbuffer, sbuffer[100];
    char command[20];
    int size;
    int index, value;

    // socket
    fsesocket::SocketClient msg_socket = fsesocket::SocketClient(centralIp, PORT);

    //setup GPIO
    gpio::setupGPIO(&msg_socket);

    while(true){

        // sending sensor data
        sensor_data = readTemperatureData(&bme280_device);
        sprintf(sbuffer, "SENSORDATA=%.2lf:%.2lf$\0", sensor_data.temperature, sensor_data.humidity);
        msg_socket.sendMessage(sbuffer, 50);

        // Check if a command was received
        rbuffer = msg_socket.receiveMessage(50, &size);

        if(size > 0){
            sscanf(rbuffer, "%s %d %d", command, &index, &value);

            std::cout << command << " " << index << " " << value << std::endl;

            // run command
            gpio::handleCommand(command, index, value);

            free(rbuffer);

            rbuffer = NULL;
        }


        // sleep time
        sleep(1);

    }

    //close socket
    msg_socket.closeSocket();

}
