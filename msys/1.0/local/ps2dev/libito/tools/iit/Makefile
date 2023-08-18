
OBJS = main.o exoquant.o swizzle.o loadimage.o saveiif.o loadbmp.o

BIN = iit

LDFLAGS  = -s # -ltiff -lpng -ljpeg
CFLAGS = -c#-DDEBUG

all : $(BIN)

%.o : %.cpp
	gcc $(CFLAGS) $< -o $@

%.o : %.c
	gcc $(CFLAGS) $< -o $@

$(BIN): $(OBJS)
	g++ -o $(BIN) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(BIN)*

run:
	./$(BIN) -p RGB24 -s script.gif test.itf