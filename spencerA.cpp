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


#ifdef SOUND
#include <sys/stat.h>
#endif //SOUND

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

void soundTrack() 
{

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./iron-curtain.mp3");
	//
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	for (int i=0; i<4; i++) {
		alSourcePlay(alSource);
		usleep(250000);
	}
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
#endif //USE_OPENAL_SOUND
	

}
