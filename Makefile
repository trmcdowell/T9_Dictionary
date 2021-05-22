
# note: -Wall flag turns on all compiler warnings

t9: t9.c
	gcc -g -Wall -o t9 t9.c

readfile: readfile.c
	gcc -Wall -o readfile readfile.c

t9-debug: t9.c
	gcc -g -Wall -o t9 t9.c

readfile-debug: readfile.c
	gcc -g -Wall -o readfile readfile.c

clean:
	rm -f t9 readfile
