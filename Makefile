all:
	#_scws.so
	gcc scws.c -shared -o _scws.so -I /usr/include/python2.6 -lscws
clean:
	rm _scws.so
