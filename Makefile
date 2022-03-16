build:
	gcc main.c -o tenis
	./tenis cerinte.in date.in rezultate.out
clean:
	rm -f tenis
