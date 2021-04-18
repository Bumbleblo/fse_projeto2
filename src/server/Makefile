

build: socket bme280
	g++ main.cpp \
		build/socket.o build/linux_userspace.o build/bme280.o \
		-Isocket/include -Ibme280/include -lpthread\
		-o project -Wno-write-strings

bdirectory:
	mkdir -p build

socket: bdirectory
	g++ -c socket/src/socket.cpp -Isocket/include -o build/socket.o

gpio: gpio/src/gpio.cpp
	g++ gpio/src/gpio.cpp -lwiringPi -Igpio/include -o gpio.o 

bme280: bdirectory
	g++ -c bme280/src/bme280.c -Ibme280/include -o build/bme280.o
	g++ -c bme280/src/linux_userspace.c -Ibme280/include -o build/linux_userspace.o

clean:
	- rm -rf build
	- rm project

run: build
	./project 10.0.0.105

zip: clean
	zip -r project.zip *
	scp project.zip rasp:~
	rm project.zip
