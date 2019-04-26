/* Source:	BenjaminG.cpp
 * @Author:	Benjamin A Garza III
 * Course:	CMPS 3350: Software Engineering
 * Assignment:	Lab 4 - Group Project Setup
 * Date:	February 14, 2019
 */

#include <iostream>
#include <unistd.h>
#include <GL/glx.h>
#include "fonts.h"
#include "core.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
extern Global& gl;
using namespace std;

/* A function that will be called from the main
 * function of our game to display my name and a picture. 
 */
void displayBenjamin(float x, float y) {
    Rect r;
    //glClear(GL_COLOR_BUFFER_BIT);
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x - 10;
    r.bot = y - 125;

    float wid = 100.0f;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glBindTexture(GL_TEXTURE_2D, gl.benImg);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();

    ggprint8b(&r, 16, 0x00ffffff, "Benjamin A Garza");
}

void displayStartScreen2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    gl.startScreen = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.startScreen);
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

}

void displayMenu() {
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
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "G - Play Game");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "O - Options");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "C - Credits Screen");

}
void displayLoadingScreen() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    gl.loadingScreen = gl.ironImage;

    glBindTexture(GL_TEXTURE_2D, gl.loadingScreen);
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
}
void displayPauseMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    gl.pauseScreen = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.pauseScreen);
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
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "C - Credits Screen");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "Y - Return to Main Screen");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "G - Play Game");

}

void displayHiddenWorld() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    gl.hiddenWorld = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.hiddenWorld);
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
}

void displayErrorScreen() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    gl.errorPage = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.errorPage);
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
}


class Leaderboard {
    public:
        int sendCall(const char[3] intials, const int score) {
            return serverConnect(intials, score);
        }
    private:
        const int MAX_READ_ERRORS = 100;
        const string USERAGENT = "CMPS-3350";
        int serverConnect(const char[3] intials, const int score) {
            BIO *ssl_setup_bio(void);
            void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
            void set_non_blocking(const int sock);
            int sd;
            struct hostent *host;
            struct sockaddr_in addr;
            BIO *outbio = NULL;
            SSL_CTX *ctx;
            SSL *ssl;
            char req[1000];
            int req_len;
            char hostname[256] = "odin.cs.csub.edu";
            char pagename[256];  
            sprintf(pagename,"/~saustin/public_html/3350/highScores.php?param=%c%c%c?param=%d",
                                 initials[0], intials[1], intials[2], score);
            int port = 443;
            int bytes, nreads, nerrs;
            char buf[256];
            int ret;
            //Setup the SSL BIO
            outbio = ssl_setup_bio();
            //Initialize the SSL library
            if (SSL_library_init() < 0)
                BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
            //Const added to match const assigned to const
            const SSL_METHOD *method = SSLv23_client_method();
            ctx = SSL_CTX_new(method);
            SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
            //next 2 lines of code are not currently needed.
            //SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
            //SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);

            //Setup the socket used for connection.
            host = gethostbyname(hostname);
            sd = socket(AF_INET, SOCK_STREAM, 0);
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = *(long*)(host->h_addr);
            if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
                BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n",
                        argv[0], hostname, inet_ntoa(addr.sin_addr), port);
            }

            //Connect using the SSL certificate.
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, sd);
            SSL_connect(ssl);

            //Send the http request to the host server.
            sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
                    pagename, hostname, USERAGENT);
            req_len = strlen(req);
            ret = SSL_write(ssl, req, req_len);

            //Correction: two statments were were separated into two lines and bracketed
            if (ret <= 0) {
                fprintf(stderr, "ERROR: SSL_write\n");
                fflush(stderr);
            }

            //Get data returned from the server.
            //First, do priming read.
            //We can take this approach because our socket is non-blocking.
            //Start with an error condition.
            bytes = -1;
            memset(buf, '\0', sizeof(buf));
            while (bytes <= 0) {
                bytes = SSL_read(ssl, buf, sizeof(buf));
                //A slight pause can cause fewer reads to be needed.
                usleep(10000);
            }
            return 0;
        }

        void set_non_blocking(const int sock) {
            //Set a socket to be non-blocking.
            int opts;
            opts = fcntl(sock, F_GETFL);
            if (opts < 0) {
                perror("ERROR: fcntl(F_GETFL)");
                exit(EXIT_FAILURE);
            }
            opts = (opts | O_NONBLOCK);
            if (fcntl(sock, F_SETFL, opts) < 0) {
                perror("ERROR: fcntl(O_NONBLOCK)");
                exit(EXIT_FAILURE);
            }
        }
}
