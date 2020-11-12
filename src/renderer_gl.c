#include "coffee.h"
#include "coffee_gl.h"
#include "toolbox.h"

boolean rgl_create_program(char *vshader, char *fshader)
{
    GLuint vertexshader, fragshader, program;

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragshader   = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexshader, 1, (const char **)&vshader, 0);
    glCompileShader(vertexshader);

    GLint cs = 0;
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &cs);
    if (cs == GL_FALSE) {
        char buf[256];
        glGetShaderInfoLog(vertexshader, 256, 0, buf);
        t_printf("VSHADER ERROR: %s\n", buf);

        glDeleteShader(vertexshader);
        return 0;
    }

    glShaderSource(fragshader, 1, (const char **)&fshader, 0);
    glCompileShader(fragshader);

    cs = 0;
    glGetShaderiv(fragshader, GL_COMPILE_STATUS, &cs);
    if (cs == GL_FALSE) {
        char buf[256];
        glGetShaderInfoLog(fragshader, 256, 0, buf);
        t_printf("FSHADER ERROR: %s\n", buf);

        glDeleteShader(fragshader);
        return 0;
    }

    program = glCreateProgram();

    glAttachShader(program, vertexshader);
    glAttachShader(program, fragshader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &cs);

    if (cs == GL_FALSE) {
        char buf[256];
        glGetProgramInfoLog(program, 256, 0, buf);
        t_printf("ERROR: %s\n", buf);
        glDeleteShader(vertexshader);
        glDeleteShader(fragshader);
        // glDeleteProgram(program);
    }

    return program;

}



