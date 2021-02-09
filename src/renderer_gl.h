#ifndef __RENDERER_GL_H
#define __RENDERER_GL_H

#include "allocator.h"
#include "common.h"
#include "platform_gl.h"
#include "result.h"

result_make_type(GLuint);

Result(GLuint) rgl_create_program_raw(Allocator * a,
                                      const char *vs,
                                      const char *fs);

#endif /* __RENDERER_GL_H */
