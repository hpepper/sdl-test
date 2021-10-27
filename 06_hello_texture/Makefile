
RUN_NAME=hello_texture
IMAGE_NAME=test_image

$(RUN_NAME): $(RUN_NAME).c test_image.bmp
	gcc  -std=c99 -o $@ $(RUN_NAME).c `sdl2-config --cflags` `sdl2-config --libs`


$(IMAGE_NAME).bmp: $(IMAGE_NAME).svg
	convert -density 96 $? $@

clean:
	- rm $(RUN_NAME)
	- rm $(IMAGE_NAME).bmp