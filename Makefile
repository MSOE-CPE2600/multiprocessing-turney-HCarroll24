CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-ljpeg

# Shared source files
JPEGRW_SOURCES=jpegrw.c
JPEGRW_OBJECTS=$(JPEGRW_SOURCES:.c=.o)

# mandel executable
MANDEL_SOURCES=mandel.c
MANDEL_OBJECTS=$(MANDEL_SOURCES:.c=.o)
MANDEL_EXECUTABLE=mandel

# mandel_movie executable
MANDEL_MOVIE_SOURCES=mandel_movie.c
MANDEL_MOVIE_OBJECTS=$(MANDEL_MOVIE_SOURCES:.c=.o)
MANDEL_MOVIE_EXECUTABLE=mandel_movie

# Default target builds both executables
all: $(MANDEL_EXECUTABLE) $(MANDEL_MOVIE_EXECUTABLE)

# pull in dependency info for *existing* .o files
-include $(JPEGRW_OBJECTS:.o=.d)
-include $(MANDEL_OBJECTS:.o=.d)
-include $(MANDEL_MOVIE_OBJECTS:.o=.d)

# Build mandel executable
$(MANDEL_EXECUTABLE): $(MANDEL_OBJECTS) $(JPEGRW_OBJECTS)
	$(CC) $(MANDEL_OBJECTS) $(JPEGRW_OBJECTS) $(LDFLAGS) -o $@

# Build mandel_movie executable
$(MANDEL_MOVIE_EXECUTABLE): $(MANDEL_MOVIE_OBJECTS) $(JPEGRW_OBJECTS)
	$(CC) $(MANDEL_MOVIE_OBJECTS) $(JPEGRW_OBJECTS) $(LDFLAGS) -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(JPEGRW_OBJECTS) $(MANDEL_OBJECTS) $(MANDEL_MOVIE_OBJECTS) $(MANDEL_EXECUTABLE) $(MANDEL_MOVIE_EXECUTABLE) *.d
