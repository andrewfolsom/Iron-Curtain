/*
 * Core Class Implementations
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "core.h"
#include "andrewF.h"
#include "chadM.h"

typedef float Flt;
typedef float Vec[3];

const int MAX_BULLETS = 1000;
const int MAX_MISSILES = 1;
extern Global& gl;

/*
 * Image Class
 */

Image::Image(const char *fname) 
{
	if (fname[0] == '\0')
		return;
	//printf("fname **%s**\n", fname);
	int ppmFlag = 0;
	char name[40];
	strcpy(name, fname);
	int slen = strlen(name);
	char ppmname[80];
	if (strncmp(name+(slen-4), ".ppm", 4) == 0)
		ppmFlag = 1;
	if (ppmFlag) {
		strcpy(ppmname, name);
	} else {
		name[slen-4] = '\0';
		//printf("name **%s**\n", name);
		sprintf(ppmname,"%s.ppm", name);
		//printf("ppmname **%s**\n", ppmname);
		char ts[100];
		//system("convert eball.jpg eball.ppm");
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
	}
	//sprintf(ts, "%s", name);
	FILE *fpi = fopen(ppmname, "r");
	if (fpi) {
		char line[200];
		fgets(line, 200, fpi);
		fgets(line, 200, fpi);
		//skip comments and blank lines
		while (line[0] == '#' || strlen(line) < 2)
			fgets(line, 200, fpi);
		sscanf(line, "%i %i", &width, &height);
		fgets(line, 200, fpi);
		//get pixel data
		int n = width * height * 3;			
		data = new unsigned char[n];			
		for (int i=0; i<n; i++)
			data[i] = fgetc(fpi);
		fclose(fpi);
	} else {
		printf("ERROR opening image: %s\n",ppmname);
		exit(0);
	}
	if (!ppmFlag)
		unlink(ppmname);
}

Image::~Image()
{
	delete [] data;
}

/*
 * Global Class
 */
Texture::Texture() {}    


Global::Global()
{
	
    xres = 900;
	yres = 1000;
	memset(keys, 0, 65536);
}

Global& Global::getInstance()
{
	static Global gl;
	return gl;
}

/*
 * Ship Class
 */

Ship::Ship() {
	pos[0] = (Flt)(gl.xres/2);
	pos[1] = (Flt)(100);
	pos[2] = 1.0f;
	vel[0] = (Flt)(0);
	vel[1] = (Flt)(0);
	vel[2] = (Flt)(0);
	vel[3] = (Flt)(0);
	speed = 0.4;
	color[0] = color[1] = color[2] = 1.0;
	detRadius = 25.0;
	wpn = new Basic;
    scnd = new Secondary;
	shield = new Shield;
}

/*
 * OpFor Class
 *	Jackson
 */
opForShip::opForShip() {

}
/*
opForShip::opForShip(const opForShip& target) {
	memcpy(pos, target.pos, 3);
	memcpy(vel, target.vel, 4);
	memcpy(color, target.color, 3);
}
*/
/*
 * Bullet Class
 */

Bullet::Bullet() { }

/*
 * Game Class
 */

Game::Game()
{
	thrustOn = false;
	barr = new Bullet[MAX_BULLETS];
	marr = new Missile[MAX_MISSILES];
	nbullets = 0;
	nmissiles = 0;
	clock_gettime(CLOCK_REALTIME, &bulletTimer);
	clock_gettime(CLOCK_REALTIME, &missileTimer);
}

Game::~Game()
{
	delete [] barr;
	delete [] marr;
}

/*
 * X11 Wrapper Class
 */

X11_wrapper::X11_wrapper()
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, 
		None};
	XSetWindowAttributes swa;
	setup_screen_res(gl.xres, gl.yres);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XWindowAttributes getWinAttr;
	XGetWindowAttributes(dpy, root, &getWinAttr);
	//
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |      
		PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease | 
		StructureNotifyMask | SubstructureNotifyMask;
	unsigned int winops = CWBorderPixel | CWColormap | CWEventMask;
	win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0, vi->depth,
			InputOutput, vi->visual, winops, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

X11_wrapper::~X11_wrapper()
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void X11_wrapper::set_title()
{
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Test Game");
}

void X11_wrapper::setup_screen_res(const int w, const int h)
{
	gl.xres = w;
	gl.yres = h;
}

void X11_wrapper::swapBuffers()
{
	glXSwapBuffers(dpy, win);
}

void X11_wrapper::clearWindow()
{
	XClearWindow(dpy, win);
}

bool X11_wrapper::getXPending()
{
	return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
}
