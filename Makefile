all: WTF WTFserver

WTF:WTF.c
	gcc WTF.c -lssl -lcrypto -o WTF

WTFserver:WTFserver.c
	gcc WTFserver.c -pthread -o WTFserver

test:WTFtest.c
	gcc WTFtest.c -o WTFtest

clean:
	rm -f WTF WTFserver WTFtest
