simulation:
	gcc src/main.c -o bin/simulation -I src/include -L src/lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2

clean:
	rm bin/simulation

run:
	make
	./bin/simulation
