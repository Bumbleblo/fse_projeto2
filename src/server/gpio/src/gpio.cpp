#include <wiringPi.h>
#include <iostream>

#include "gpio.hpp"

void sensorPresenca01Notification(){notification(25);}
void sensorPresenca02Notification(){notification(26);}

void sensorAbertura01Notification(){notification(5);}
void sensorAbertura02Notification(){notification(6);}
void sensorAbertura03Notification(){notification(12);}
void sensorAbertura04Notification(){notification(16);}
void sensorAbertura05Notification(){notification(20);}
void sensorAbertura06Notification(){notification(21);}


void setupInterruption(int pin, void(*callback)(void))
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

void setupGPIO(int socket){

    gpio_socket = socket;

    setupInterruption(25, sensorPresenca01Notification);
    setupInterruption(26, sensorPresenca02Notification);

    setupInterruption(5, sensorAbertura01Notification);
    setupInterruption(6, sensorAbertura02Notification);
    setupInterruption(12, sensorAbertura03Notification);
    setupInterruption(16, sensorAbertura04Notification);
    setupInterruption(20, sensorAbertura05Notification);
    setupInterruption(21, sensorAbertura06Notification);

}

void notification(int pin){
    std::cout << "PIN MESSAGE " << pin << std::endl;
}


int main(){

    wiringPiSetupGpio();

    setupGPIO(4);

    while(1){
    }


}
