#include <wiringPi.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include "gpio.hpp"
#include "socket.hpp"

using namespace gpio;

fsesocket::SocketClient *client;

void gpio::sensorPresenca01Notification(){notification(25);}
void gpio::sensorPresenca02Notification(){notification(26);}

void gpio::sensorAbertura01Notification(){notification(5);}
void gpio::sensorAbertura02Notification(){notification(6);}
void gpio::sensorAbertura03Notification(){notification(12);}
void gpio::sensorAbertura04Notification(){notification(16);}
void gpio::sensorAbertura05Notification(){notification(20);}
void gpio::sensorAbertura06Notification(){notification(21);}


void gpio::setupInterruption(int pin, void(*callback)(void))
{

    pinMode(1, INPUT);

    int setup_r = wiringPiISR(
        pin,
        INT_EDGE_FALLING,
        callback
    );

    if(setup_r < 0){
        std::cerr << "Setup pin " << pin << "failed" << std::endl;
        exit(20);
    }

}

void gpio::setupOutput()
{
    pinMode(17, OUTPUT);
    digitalWrite(17, 0);

    pinMode(18, OUTPUT);
    digitalWrite(18, 0);

    pinMode(27, OUTPUT);
    digitalWrite(27, 0);

    pinMode(22, OUTPUT);
    digitalWrite(22, 0);

    pinMode(13, OUTPUT);
    digitalWrite(13, 0);

    pinMode(19, OUTPUT);
    digitalWrite(19, 0);
}

void gpio::handleCommand(char *command, int index, int value)
{

    if(strcmp(command, "LAMP") == 0){

        switch(index){
            case 1:
                digitalWrite(17, value);
            break;

            case 2:
                digitalWrite(18, value);
            break;

            case 3:
                digitalWrite(27, value);
            break;

            case 4:
                digitalWrite(22, value);
            break;

            default:
                std::cerr << "Lamp index out of range" << std::endl;
        }

    }else if(strcmp(command, "AR") == 0){

        switch(index){
            case  1:
                digitalWrite(13, value);
            break;

            case 2:
                digitalWrite(19, value);
            break;

            default:
                std::cerr << "AR index out of range" << std::endl;

        }
    }else{
        std::cerr << "Comando inválido" << std::endl;
    }

}

void gpio::setupGPIO(fsesocket::SocketClient *s){

    int r = wiringPiSetupGpio();

    client = s;

    setupInterruption(25, sensorPresenca01Notification);
    setupInterruption(26, sensorPresenca02Notification);

    setupInterruption(5, sensorAbertura01Notification);
    setupInterruption(6, sensorAbertura02Notification);
    setupInterruption(12, sensorAbertura03Notification);
    setupInterruption(16, sensorAbertura04Notification);
    setupInterruption(20, sensorAbertura05Notification);
    setupInterruption(21, sensorAbertura06Notification);

    setupOutput();

}

void gpio::notification(int pin){
    std::cout << "PIN MESSAGE " << pin << std::endl;

    char buffer[100];

    sprintf(buffer, "EVENT=%d$\0", pin);

    std::cout << buffer << std::endl;

    client->sendMessage(buffer, 50);

}
