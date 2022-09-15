#ifndef _UTILS_H
#define _UTILS_H

#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>

unsigned char* loadBitmap(const char filename[], int width, int height);
GLchar* loadShader(const char filename[]);
void logShaderInfo(GLuint shader_handle, const char msg[]);
void linkProgram(GLuint program_handle);
void validateProgram(GLuint program_handle);

#endif