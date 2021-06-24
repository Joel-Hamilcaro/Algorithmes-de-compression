CC=gcc
CFLAGS=-g -Wall

huffman: bin/tree.o bin/list.o bin/huffman.o
	$(CC) $(CFLAGS) -o $@ $^

hufDy: bin/list.o bin/hufDy.o
	$(CC) $(CFLAGS) -o $@ $^

deHufDy: bin/list.o bin/deHufDy.o
	$(CC) $(CFLAGS) -o $@ $^

newZl: bin/list.o bin/newZl.o
	$(CC) $(CFLAGS) -o $@ $^

deZl: bin/list.o bin/deZl.o
	$(CC) $(CFLAGS) -o $@ $^

bin/tree.o: huffmanStatique/tree.c huffmanStatique/tree.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/list.o: list/list.c list/list.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/huffman.o: huffmanStatique/huffman.c huffmanStatique/huffman.h list/list.h huffmanStatique/tree.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/hufDy.o: huffmanDynamique/hufDy.c
	$(CC) $(CFLAGS) -c -o $@ $<

bin/deHufDy.o: huffmanDynamique/deHufDy.c
	$(CC) $(CFLAGS) -c -o $@ $<

bin/newZl.o: zivLempel/newZl.c
	$(CC) $(CFLAGS) -c -o $@ $<

bin/deZl.o: zivLempel/deZl.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *~ bin/*.o huffman hufDy deHufDy newZl deZl
