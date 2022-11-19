simulation:
	gcc src/main.c -o bin/simulation -I src/include -L src/lib -l SDL2-2.0.0

clean:
	rm bin/simulation

run:
	make
	./bin/simulation