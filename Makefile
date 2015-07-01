all:
	gcc -m32 -shared -Wall -I. xpending.c login.c -o tibia-hook.so
