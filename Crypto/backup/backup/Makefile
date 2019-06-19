out: main.o encode_decode.o
	cc -o out main.o encode_decode.o

main.o: main.c encode_decode.h
	cc -c main.c

encode_decode.o: encode_decode.c encode_decode.h
	cc -c encode_decode.c

clean:
	@rm -f *.o out
