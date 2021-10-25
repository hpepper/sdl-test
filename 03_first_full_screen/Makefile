
RUN_NAME=first_full_screen

$(RUN_NAME): $(RUN_NAME).c
	gcc  -std=c99 -o $@ $? `sdl2-config --cflags` `sdl2-config --libs`

clean:
	- rm $(RUN_NAME)