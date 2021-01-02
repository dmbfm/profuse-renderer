#include "renderer_gl.h"
#include "allocator.h"
#include "error.h"
#include "slice.h"
#include "utils.h"

typedef enum
{
    PRINT_ERROR_SHADER,
    PRINT_ERROR_PROGRAM
} PrintShaderErrorMessageType;

void _rgl_print_shader_error_message(Allocator *a, GLuint handle, PrintShaderErrorMessageType type)
{
    GLint len;
    if (type == PRINT_ERROR_SHADER)
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
    else
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);

#if defined(_MSC_VER)
    Result(uptr) rbuff = a->alloc(a, len);
    char *buf          = (char *)result_unwrap(rbuff);
#else
    char buf[len];
#endif /* defined(_MSC_VER) */

    if (type == PRINT_ERROR_SHADER) {
        glGetShaderInfoLog(handle, len, 0, buf);
        DEBUGLOG("Failed to compile shader: %s", buf);
    } else {
        glGetProgramInfoLog(handle, len, 0, buf);
        DEBUGLOG("Failed to link program: %s", buf);
    }
#if defined(_MSC_VER)
    a->free(a, (uptr)buf);
#endif /* defined(_MSC_VER) */
}

Result(GLuint) rgl_compile_shader_raw(Allocator *a, GLuint shader_type, const char *shader_source)
{
    GLuint shader;

    shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, (const char **)&shader_source, 0);
    glCompileShader(shader);

    GLint cs = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &cs);
    if (cs == GL_FALSE) {
        _rgl_print_shader_error_message(a, shader, PRINT_ERROR_SHADER);
        glDeleteShader(shader);
        return result_error(GLuint, ERR_RGL_VERTEX_SHADER);
    }

    return result_ok(GLuint, shader);
}

Result(GLuint) rgl_create_program_raw(Allocator *a, const char *vshader, const char *fshader)
{
    GLuint vertexshader, fragshader, program;
    GLint cs;
    Result(GLuint) r;

    r = rgl_compile_shader_raw(a, GL_VERTEX_SHADER, vshader);
    result_raise(r);
    vertexshader = r.value;

    r = rgl_compile_shader_raw(a, GL_FRAGMENT_SHADER, fshader);
    result_raise(r);
    fragshader = r.value;

    program = glCreateProgram();

    glAttachShader(program, vertexshader);
    glAttachShader(program, fragshader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &cs);

    if (cs == GL_FALSE) {
        _rgl_print_shader_error_message(a, program, PRINT_ERROR_PROGRAM);

        glDeleteShader(vertexshader);
        glDeleteShader(fragshader);
        glDeleteProgram(program);

        return result_error(GLuint, ERR_RGL_VERTEX_SHADER);
    }

    return result_ok(GLuint, program);
}
