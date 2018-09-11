CC = g++
CFLAGS = -g3
TAG = ctags
TAGFLAGS = -R
all: main.o utility.o autMgr.o aut.o vmtnode.o tgraph.o
	$(CC) $(CFLAGS) main.o utility.o autMgr.o aut.o vmtnode.o tgraph.o -o ../bin/slender
	$(TAG) $(TAGFLAGS)
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c
utility.o: utility.cpp
	$(CC) $(CFLAGS) utility.cpp -c
autMgr.o: autMgr.cpp
	$(CC) $(CFLAGS) autMgr.cpp -c
aut.o: aut.cpp
	$(CC) $(CFLAGS) aut.cpp -c
vmtnode.o: vmtnode.cpp
	$(CC) $(CFLAGS) vmtnode.cpp -c
tgraph.o: tgraph.cpp
	$(CC) $(CFLAGS) tgraph.cpp -c
clean:
	rm *.o tags ../bin/slender
