EXECUTABLE = romdemo
SHARED_LIB = libprogressbar.so
STATIC_LIB = libprogressbar.a

SRC=lib
INCLUDE=include/progressbar
TEST=test
CFLAGS += -std=c99 -I$(INCLUDE) -Wimplicit-function-declaration -Wall -Wextra -pedantic
CFLAGS_DEBUG = -g -O0
LDLIBS = -lncurses

all: $(EXECUTABLE) $(STATIC_LIB) # $(SHARED_LIB)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(EXECUTABLE)

doc: $(INCLUDE)/progressbar.h $(INCLUDE)/statusbar.h
	mkdir -p doc
	doxygen

$(EXECUTABLE): romdemo.o test_mtx.o progressbar.o statusbar.o

libprogressbar.so: $(INCLUDE)/progressbar.h $(SRC)/progressbar.c
	$(CC) -fPIC -shared -o $@ -c $(CFLAGS) $(CPPFLAGS) $(SRC)/progressbar.c

libprogressbar.a: libprogressbar.a(progressbar.o)

%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

romdemo.o: CFLAGS += -std=gnu99 # Demo uses usleep which requires POSIX or BSD source
romdemo.o: $(TEST)/main.c 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o romdemo.o

test_mtx.o: CFLAGS += -std=c11
test_mtx.o: $(TEST)/test_mtx.c 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o test_mtx.o

.PHONY: clean
clean:
	rm -f *.o $(EXECUTABLE) $(STATIC_LIB) $(SHARED_LIB)
	rm -rf doc
