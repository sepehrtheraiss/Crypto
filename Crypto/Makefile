out: main.o encode.o 
	cc -o out main.o encode.o 
	@rm main.o encode.o

main.o: main.c encode.h 
	cc -c main.c

#functions.o: functions.c functions.h
#	cc -c functions.c

encode.o: encode.c encode.h
	cc -c encode.c

clean:
	@rm -f *.o out
