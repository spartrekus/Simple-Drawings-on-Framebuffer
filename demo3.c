
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>


///#include "framebuffer.h"
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <linux/fb.h>

typedef struct framebuffer{
	int framebufferfd;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	char *fb_memp;

	long int xmax;
	int xoffset;
	long int ymax;
	int yoffset;
	int bpp; //bits per pixel
	long int screensize;
} FrameBuffer_t;

//typedef struct framebuffer FrameBuffer;

void FrameBufferOpen(FrameBuffer_t *frame, int framebufferfd);

void FrameBufferWritePixel(FrameBuffer_t *frame, long int x, long int y, int red, int blue, int green, int transparency);

void FrameBufferClose(FrameBuffer_t *frame);

long int FrameBufferLocation(FrameBuffer_t *frame, long int x, long int y);

#endif




int main()
{
	int fbfd = 0;

	FrameBuffer_t *frame;
	frame = malloc(sizeof(frame));

	FILE *stream;
	stream = stdout;

	struct termios old_termios, new_termios;

	// Open framebuffer for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }

    
    //hide cursor
    fputs("\e[?25l",stdout);

    //turn off terminal behaviour
    if (tcgetattr (fileno (stream), &old_termios) != 0)
    	return -1;
	new_termios = old_termios;
	new_termios.c_lflag &= ~(ECHO);
	if (tcsetattr (fileno (stream), TCSAFLUSH, &new_termios) != 0)
		return -1;
	

    FrameBufferOpen(frame, fbfd);

    int x,y;
    for(x = 0; x < frame->xmax; x++) {
    	for(y = 0; y < frame->ymax; y++) {
    		FrameBufferWritePixel(frame, x, y, 0, 0, 0, 100);
    	}
    }

    for(x = 0; x < frame->xmax; x++) {
    	for(y = 0; y < frame->ymax; y++) {
    		//FrameBufferWritePixel(frame, x, y, 255, 0, 0, 100);
    		FrameBufferWritePixel(frame, x, y, rand() % 255, rand() % 255, rand() % 255, 100);
    	}
    }

    getchar();

    for(x = 0; x < frame->xmax; x++) {
    	for(y = 0; y < frame->ymax; y++) {
    		FrameBufferWritePixel(frame, x, y, 0, 0, 0, 100);
    	}
    }

    //perform clean up and show cursor
    FrameBufferClose(frame);
    close(fbfd);
    
    
    //turn on terminal behaviour
    (void) tcsetattr (fileno (stream), TCSAFLUSH, &old_termios);

    //show cursor
    fputs("\e[?25h",stdout);
	

    return 0;
}
