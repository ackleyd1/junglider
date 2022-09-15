#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "utils.h"

#define WIDTH 700
#define HEIGHT 500
#define WINDOW_TITLE "Junglider"
#define VS_FILENAME "assets/shaders/test_vs.glsl"
#define FS_FILENAME "assets/shaders/test_fs.glsl"
#define BMP_FILENAME "assets/bmps/logo2.bmp"

GLuint shader_program;
GLuint idx = 0;
GLuint vao = 0;
GLuint vbo = 0;
GLuint vs = 0;
GLuint fs = 0;

void init() {
    // set the color to clear the framebuffer to
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // create empty shader program object and initialize its handle
    shader_program = glCreateProgram();

    // create empty vertex and fragment shader objects and initialize handles
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    // load, compile and log vertex shader
    GLchar* vs_src = loadShader(VS_FILENAME);
    glShaderSource(vs, 1, (const GLchar * const*) &vs_src, NULL);
    glCompileShader(vs);
    logShaderInfo(vs, "Vertex");

    // load, compile and log fragment shader
    GLchar* fs_src = loadShader(FS_FILENAME);
    glShaderSource(fs, 1, (const GLchar * const*) &fs_src, NULL);
    glCompileShader(fs);
    logShaderInfo(fs, "Fragment");

    free(vs_src);
    free(fs_src);

    // attach compiled shader objects to the program
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);

    // bind the attribute 'pos' to location idx
    glBindAttribLocation(shader_program, idx, "pos");

    // create executable for the vertex processor
    linkProgram(shader_program);

    // generate vertex arrays and buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);

    glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);

    // enable blending for the texture alpha channel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint textureID;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);

    unsigned char* raster = loadBitmap(BMP_FILENAME, WIDTH, HEIGHT, 4);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // bitmap format is BGR
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
         WIDTH, HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, raster);

    free(raster);

    // projection matrix values
    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    // calculate vertex and texture coordinates
    // bottom left y coord inverted
    float x = -1;
    float y = -1;
    float w = WIDTH * sx;
    float h = HEIGHT * sy;

    // vertices are top left, top right, bottom left, bottom right
    // v0, v1, v2 then v2, v1, v3 then v2, v3, v4 ... then v4, v3, v5, then v4, v5, v6
    // here we have two triangles
    // triangle 1 is top left, top right, bottom left
    // triangle 2 is bottom left, top right, bottom right
    // are surface normals interpreted properly from the windings?
    GLfloat box[4][4] = {
        {x, -y, 0, 0},
        {x + w, -y, 1, 0},
        {x, -y - h, 0, 1},
        {x + w, -y - h, 1, 1},
    };

    // bind vao for vertex array objects
    glBindVertexArray(vao);

    // enable the attribute array for current vao
    glEnableVertexAttribArray(idx);

    // bind vbo for vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // construct the buffer data for vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_STATIC_DRAW);

    // specify location and data format for vertex attributes
    glVertexAttribPointer(idx, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // draw from the currently bound vao
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // render directly to the front buffer of the default framebuffer
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(WINDOW_TITLE);
    glutDisplayFunc(display);

    // initialize glew
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

    init();
    glViewport(0, 0, WIDTH, HEIGHT);
    glutMainLoop();

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(shader_program);
}