#ifndef __GPIO__
#define __GPIO__

#include "socket.hpp"

namespace gpio{


void sensorPresenca01Notification();
void sensorPresenca02Notification();

void sensorAbertura01Notification();
void sensorAbertura02Notification();
void sensorAbertura03Notification();
void sensorAbertura04Notification();
void sensorAbertura05Notification();
void sensorAbertura06Notification();

void setupInterruption(int, void(*f)(void));
void setupGPIO(fsesocket::SocketClient*);

void handleCommand(char *, int, int);
void setupOutput();


void notification(int);

}
#endif
