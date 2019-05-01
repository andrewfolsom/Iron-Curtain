#include <GL/glx.h>
#include <stdio.h>
#include <cmath>
#include "fonts.h"
#include "core.h"
#include <iostream>
#include <GL/glu.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

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

    glBindTexture(GL_TEXTURE_2D, gl.gameControls);
    glBegin(GL_QUADS);
         glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
         glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
         glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
         glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);

	glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c, "C - Credits Screen");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c,"Y - Return to Main Screen");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c,"G - Play Game");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c, "Z - Game Controls");
} 



void displayGameControls()
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

	glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c, "C - Credits Screen");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c,"G/g - Play Game");
    ggprint16(&r, 16,c," ");
    ggprint16(&r, 16,c,"Y/y - Return to Main Screen");

    Rect r2;
    unsigned int d = 0x00000000;
    r2.bot = gl.yres-500;
    r2.left = 350;
    r2.center = 0;
    
        
    ggprint16(&r2, 16,d,"****** Game Controls****** ");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"A/a ------ Move left");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"D/d ------ Move right");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"W/w ------ Move up");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"X/x ------ Move down");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"Spacebar ------ Shoot");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"M/m -------- Lock On Target");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"B/b ----------- Shield");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"1 ----------- Basic Weapon");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"2 ----------- Rapid Fire");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"3 ----------- Scatter Fire");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"4 ----------- Ring Spatial Fire");
    ggprint16(&r2, 16,d," ");
    ggprint16(&r2, 16,d,"5 ----------- Pinwhell Fire");


    
}
void scrollingBackground()
{
  
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

void soundTrack() {
	
	#ifdef USE_OPENAL_SOUND

        alutInit(0, NULL);
        if (alGetError() != AL_NO_ERROR) {
                printf("ERROR: alutInit()\n");
                return ;
        }

        alGetError();

        float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
        alListenerfv(AL_ORIENTATION, vec);
        alListenerf(AL_GAIN, 1.0f);

        ALuint alBuffer[2];
        alBuffer[0] = alutCreateBufferFromFile("./sounds/Battlefield 1942(2).wav");
       
        alBuffer[1] = alutCreateBufferFromFile("./sounds/Battlefield 1942(2).wav");

        ALuint alSource[2];

        alGenSources(2, alSource);
        alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
        
        alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
        
        alSourcef(alSource[0], AL_GAIN, 1.0f);
        alSourcef(alSource[0],AL_PITCH, 1.0f);

	  alSourcei(alSource[0], AL_LOOPING, AL_TRUE);
        if (alGetError() != AL_NO_ERROR) {
                printf("ERROR: setting source\n");
                return;
        }

	 alSourcef(alSource[1], AL_GAIN, 0.5f);
        alSourcef(alSource[1], AL_PITCH, 1.0f);
        alSourcei(alSource[1], AL_LOOPING, AL_TRUE);
        if (alGetError() != AL_NO_ERROR) {
                printf("ERROR: setting source\n");
                return ;
        }

	alSourcePlay(alSource[1]);
        for (int i=0; i<42; i++) {
                alSourcePlay(alSource[0]);
               // usleep(250000);
        }
        	
#endif //USE_OPENAL_SOUND
    
}

