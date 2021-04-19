

build: socket bme280 gpio
	g++ main.cpp \
		build/socket.o build/linux_userspace.o build/bme280.o build/gpio.o\
		-Isocket/include -Ibme280/include -Igpio/include -lwiringPi -lpthread\
		-o project -Wno-write-strings

bdirectory:
	mkdir -p build

socket: bdirectory
	g++ -c socket/src/socket.cpp -Isocket/include -o build/socket.o

gpio: gpio/src/gpio.cpp bdirectory
	g++ -c gpio/src/gpio.cpp \
		-lwiringPi -Igpio/include -Isocket/include -o build/gpio.o 

bme280: bdirectory
	g++ -c bme280/src/bme280.c -Ibme280/include -o build/bme280.o
	g++ -c bme280/src/linux_userspace.c -Ibme280/include -o build/linux_userspace.o

clean:
	- rm -rf build
	- rm project

run:
	./project $(central_ip)

zip: clean
	zip -r project.zip *
	scp project.zip rasp:~
	rm project.zip
