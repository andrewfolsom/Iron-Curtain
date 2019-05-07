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

    Rect r3;
    unsigned int d = 0x00000000;
    r3.bot = gl.yres-400;
    r3.left = 350;
    r3.center = 0;
    ggprint16(&r3, 16,d,"****** How To Play ****** ");
    ggprint16(&r3, 16,d," ");
    
    Rect r2;
    unsigned int e = 0x00000000;
    r2.bot = gl.yres-500;
    r2.left = 10;

    r2.center = 0;
    ggprint16(&r2, 16,e,"---Basic Ship Movement---");
    ggprint16(&r2, 16,e," ");
    ggprint16(&r2, 16,e,"A/a ------ Move left");
    ggprint16(&r2, 16,e," ");
    ggprint16(&r2, 16,e,"D/d ------ Move right");
    ggprint16(&r2, 16,e," ");
    ggprint16(&r2, 16,e,"W/w ------ Move up");
    ggprint16(&r2, 16,e," ");
    ggprint16(&r2, 16,e,"X/x ------ Move down");
    ggprint16(&r2, 16,e," ");
    
    Rect r4;
    unsigned int f = 0x00000000;
    r4.bot = gl.yres-500;
    r4.left = 350;
    r4.center = 0;
    ggprint16(&r4, 16,f,"--- How to Kill Enemies---");
    ggprint16(&r4, 16,f," ");
    ggprint16(&r4, 16,f,"Spacebar ------ Shoot");
    ggprint16(&r4, 16,f," ");
    ggprint16(&r4, 16,f,"1 ----------- Basic Weapon");
    ggprint16(&r4, 16,f," ");
    ggprint16(&r4, 16,f,"2 ----------- Rapid Fire");
    ggprint16(&r4, 16,f," ");
    ggprint16(&r4, 16,f,"3 ----------- Scatter Fire");
    ggprint16(&r4, 16,f," ");
    ggprint16(&r4, 16,f,"4 ----------- Ring Spatial Fire");
    ggprint16(&r4, 16,f," ");
    ggprint16(&r4, 16,f,"5 ----------- Pinwhell Fire");

    Rect r5;
    unsigned int g = 0x00000000;
    r5.bot = gl.yres-500;
    r5.left = 650;
    r5.center = 0;
    ggprint16(&r5, 16,g,"---- Special Mods ----");
    ggprint16(&r5, 16,g," ");
    ggprint16(&r5, 16,g,"M/m -------- Lock On Target");
    ggprint16(&r5, 16,g," ");
    ggprint16(&r5, 16,g,"B/b ----------- Shield");
    ggprint16(&r5, 16,g," ");
    
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


void scrollingBackground2()
{  
   // glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    	

    glBindTexture(GL_TEXTURE_2D, gl.clouds);
    glBegin(GL_QUADS);
		glTexCoord2f(gl.tex.xc[0], gl.tex.yc[0]); glVertex2i(0, 0);
		glTexCoord2f(gl.tex.xc[0], gl.tex.yc[0]); glVertex2i(0, gl.yres);
		glTexCoord2f(gl.tex.xc[0], gl.tex.yc[1]); glVertex2i(gl.xres, gl.yres);
		glTexCoord2f(gl.tex.xc[1], gl.tex.yc[1]); glVertex2i(gl.xres, 0);

	glEnd();

}


void soundTrack() 
{	
	#ifdef USE_OPENAL_SOUND

       //alutInit(0, NULL);
        if (alGetError() != AL_NO_ERROR) {
                
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

void cannonFire() 
{
#ifdef USE_OPENAL_SOUND

	alGetError();
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	ALuint alBuffer[2];
	alBuffer[0] = alutCreateBufferFromFile("./sounds/cannonFire.wav");
	ALuint alSource[1];
	alGenSources(1, alSource);
	alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
	alSourcef(alSource[0], AL_GAIN, 1.0f);
	alSourcef(alSource[0], AL_PITCH, 1.0f);
	alSourcei(alSource[0], AL_LOOPING, AL_FALSE);

		if (alGetError() != AL_NO_ERROR) {

		printf("ERROR: setting source\n");

		return ;

	}

	alSourcePlay(alSource[1]);
	for (int i=0; i<1; i++) {
		alSourcePlay(alSource[0]);
	}

#endif //USE_OPENAL_SOUND
	return;
}


void explodeShip() 
{
#ifdef USE_OPENAL_SOUND

	alGetError();
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	ALuint alBuffer[2];
	alBuffer[0] = alutCreateBufferFromFile("./sounds/Explosion+3.wav");
	ALuint alSource[1];
	alGenSources(1, alSource);
	alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
	alSourcef(alSource[0], AL_GAIN, 1.0f);
	alSourcef(alSource[0], AL_PITCH, 1.0f);
	alSourcei(alSource[0], AL_LOOPING, AL_FALSE);

		if (alGetError() != AL_NO_ERROR) {

		printf("ERROR: setting source\n");

		return ;

	}

	alSourcePlay(alSource[1]);
	for (int i=0; i<1; i++) {
		alSourcePlay(alSource[0]);
	}

#endif //USE_OPENAL_SOUND
	return;
}

