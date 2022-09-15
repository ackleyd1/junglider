#include "utils.h"

unsigned char* loadBitmap(const char filename[], int width, int height, int depth) {
    FILE* fd = fopen(filename, "rb");
    if (fd == NULL) {
        // TODO: destroy shader objects before exit
        exit(-1);
    }

    // bmp images are stored upside down
    // using pointer arithmetic to flip it back up
    unsigned char * raster = (unsigned char *) malloc(width * height * depth);
    fseek(fd, 138, 0); // bitmap header is 54 bytes
    for (int row = height-1; row >= 0; row--) {
        fread(raster + row * width * depth, width * depth, 1, fd);
    }
    fclose(fd);

    return raster;
}

// load shader source from a file
// TODO: hot reload using inotify
GLchar* loadShader(const char filename[]) {
    // open file and get length
    FILE* fd = fopen(filename, "rb");
    if(fd == NULL) {
        // TODO: destroy shader objects before exit
        fprintf(stderr, "ERROR: could not open file %s\n", filename);
        exit(-1);
    }

    fseek(fd, 0, SEEK_END);
    long length = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    // read data into buffer and close file
    GLchar * buffer = (GLchar *) malloc(length);
    fread(buffer, 1, length, fd);
    fclose(fd);
    return buffer;
}

// check compile status of shader object and log errors
void logShaderInfo(GLuint shader_handle, const char msg[]) {
    int params = -1;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE) {
        fprintf(stderr, "ERROR: %s shader index %u did not compile\n", msg, shader_handle);

        char info[1024];
        int length = 0;
        glGetShaderInfoLog(shader_handle, sizeof info, &length, info);
        if(length) {
            fprintf(stderr, "shader info log for GL index: %u\n%s\n", shader_handle, info);
        }
    }
}

// check link status of shader program and log errors
void linkProgram(GLuint program_handle) {
    int params = -1;
    glLinkProgram(program_handle);
    glGetProgramiv(program_handle, GL_LINK_STATUS, &params);
    if (params != GL_TRUE) {
        fprintf(stderr, "ERROR: link for shader program index %u was unsuccessful\n", program_handle);

        char info[1024];
        int length = 0;
        glGetProgramInfoLog(program_handle, sizeof info, &length, info);
        if(length) {
            fprintf(stderr, "shader program info log for GL index: %u\n%s\n", program_handle, info);
        }
    }
}

// determine if the program can execute given the current OpenGL state
void validateProgram(GLuint program_handle) {
    int params = -1;
    glValidateProgram(program_handle);
    glGetProgramiv(program_handle, GL_VALIDATE_STATUS, &params);
    if (params != GL_TRUE) {
        fprintf(stderr, "ERROR: shader program index %u is invalid\n", program_handle);

        char info[1024];
        int length = 0;
        glGetProgramInfoLog(program_handle, sizeof info, &length, info);
        if(length) {
            fprintf(stderr, "shader program info log for GL index: %u\n%s\n", program_handle, info);
        }
    }
}