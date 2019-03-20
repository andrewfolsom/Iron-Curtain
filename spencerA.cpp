#include <GL/glx.h>
#include <stdio.h>
#include <cmath>
#include "fonts.h"
#include "core.h"
#include <iostream>
#include <GL/glu.h>
extern Global& gl;

void displaySpencer(float x, float y, GLuint texture)
{
    float wid = 100.0f;
    unsigned int color = 0x00bfb418;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();
    
    Rect r;
    r.width = 250.0;
    r.height = 100.0;
    r.center = 0;
    r.left = x-65;
    r.bot = y- 125;

    ggprint16(&r, 16, color, "Spencer Austin");
}

void displayStartScreen()
{  
    initialize_fonts();
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    glBindTexture(GL_TEXTURE_2D, gl.ironImage);
    glBegin(GL_QUADS);
         glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
         glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
         glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
         glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
	/*	glTexCoord2f(gl.tex.xc[0], gl.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(gl.tex.xc[0], gl.tex.yc[0]); glVertex2i(0, gl.yres);
		glTexCoord2f(gl.tex.xc[1], gl.tex.yc[0]); glVertex2i(gl.xres, gl.yres);
		glTexCoord2f(gl.tex.xc[1], gl.tex.yc[1]); glVertex2i(gl.xres, 0);
*/
	glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c, "C - Credits Screen");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c,"G/g - Return to Main Screen");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c,"P - Play Game");
} 

void scrollingBackground(){
  
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    glBindTexture(GL_TEXTURE_2D, gl.verticalBackground);
    glBegin(GL_QUADS);
		glTexCoord2f(gl.tex.xc[1], gl.tex.yc[0]); glVertex2i(0, 0);
		glTexCoord2f(gl.tex.xc[0], gl.tex.yc[0]); glVertex2i(0, gl.yres);
		glTexCoord2f(gl.tex.xc[0], gl.tex.yc[1]); glVertex2i(gl.xres, gl.yres);
		glTexCoord2f(gl.tex.xc[1], gl.tex.yc[1]); glVertex2i(gl.xres, 0);

	glEnd();

}
