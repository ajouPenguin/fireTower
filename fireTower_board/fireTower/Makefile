main : main.o bitmap.o touch.o dipsw.o led.o buzzer.o mled.o cled.o tlcd.o oled.o keypad.o camera.o fnd.o login.o comm_with_arduino.o comm_with_server.o thread_touchLCD.o thread_touch.o thread_etcPeri.o thread_communication.o
	arm-none-linux-gnueabi-gcc -pthread -o main main.o touch.o bitmap.o dipsw.o led.o buzzer.o mled.o cled.o tlcd.o oled.o keypad.o camera.o fnd.o login.o comm_with_arduino.o comm_with_server.o thread_touchLCD.o thread_touch.o thread_etcPeri.o thread_communication.o
	cp main /tftp

main.o : main.c
	arm-none-linux-gnueabi-gcc -c main.c

thread_touchLCD.o : thread_touchLCD.c touch.o bitmap.o camera.o keypad.o login.o thread_touch.o
	arm-none-linux-gnueabi-gcc -c thread_touchLCD.c
thread_touch.o : thread_touch.c touch.o
	arm-none-linux-gnueabi-gcc -c thread_touch.c
thread_etcPeri.o : thread_etcPeri.c
	arm-none-linux-gnueabi-gcc -c thread_etcPeri.c
thread_communication.o : thread_communication.c comm_with_arduino.o comm_with_server.o
	arm-none-linux-gnueabi-gcc -c thread_communication.c

touch.o : touch.c
	arm-none-linux-gnueabi-gcc -c touch.c
dipsw.o : dipsw.c
	arm-none-linux-gnueabi-gcc -c dipsw.c
led.o : led.c
	gcc -c led.c
buzzer.o : buzzer.c
	arm-none-linux-gnueabi-gcc -c buzzer.c
mled.o : mled.c
	arm-none-linux-gnueabi-gcc -c mled.c
cled.o : cled.c
	arm-none-linux-gnueabi-gcc -c cled.c
tlcd.o : tlcd.c
	arm-none-linux-gnueabi-gcc -c tlcd.c
oled.o : oled.c
	arm-none-linux-gnueabi-gcc -c oled.c
login.o : login.c keypad.o bitmap.o
	arm-none-linux-gnueabi-gcc -c login.c
keypad.o : keypad.c
	arm-none-linux-gnueabi-gcc -c keypad.c
camera.o : camera.c
	arm-none-linux-gnueabi-gcc -c camera.c
fnd.o : fnd.c
	arm-none-linux-gnueabi-gcc -c fnd.c
bitmap.o : bitmap.c
	arm-none-linux-gnueabi-gcc -c bitmap.c

comm_with_arduino.o : comm_with_arduino.c
	arm-none-linux-gnueabi-gcc -c comm_with_arduino.c
comm_with_server.o : comm_with_server.c
	arm-none-linux-gnueabi-gcc -c comm_with_server.c


clean :
	rm -f *.o *~ main
