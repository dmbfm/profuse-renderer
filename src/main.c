//#define __TG_RUN_TESTS

#ifndef __wasm__
#include <stdio.h>
#include <stdlib.h>
#endif

#define __TG_RUN_TESTS

#include "tg_debug.h"
#include "tg_platform.h"
#include "tg.h"
#include "tg_math.h"
#include "tg_wasm.h"

#include "tg_gl.h"

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "external/tinyobj_loader_c.h"

// START: Sketch renderer
typedef struct Mesh
{
    f32 *vertices;
    f32 *normals;
    f32 *uvs;
    u16 *indices;

    int num_vertices;
    int num_indices;

    Vec3 position;

    // TEMP
    u32 vbo;
    u32 ebo;
} Mesh;

void mesh_load(Mesh *mesh)
{
    tg_assert(mesh->vertices);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

    int buffer_size = 3 * 4 * mesh->num_vertices;

    if (mesh->normals)
        buffer_size *= 2;

    if (mesh->uvs)
        buffer_size += 8 * mesh->num_vertices;

    glBufferData(GL_ARRAY_BUFFER, buffer_size, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 4 * mesh->num_vertices, (const void *) mesh->vertices);

    if (mesh->normals)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 12 * mesh->num_vertices, 12 * mesh->num_vertices, (const void *) mesh->normals);
    }

    if (mesh->uvs)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 24 * mesh->num_vertices, 8 * mesh->num_vertices, (const void *) mesh->uvs);
    }


    if (mesh->indices)
    {
        glGenBuffers(1, &mesh->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * mesh->num_indices, (const void *) mesh->indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh_draw(Mesh *mesh)
{
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(0);

        if (mesh->normals) 
        {
            glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, (const void *) (u64) (12 * mesh->num_vertices));
            glEnableVertexAttribArray(1);
        }

        if (mesh->uvs) 
        {
            glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, (const void *) (u64) (24 * mesh->num_vertices));
            glEnableVertexAttribArray(2);
        }

        if (!mesh->indices) {
            glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
            glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_SHORT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);
}

const float triangle_vertices[] = { 
    0, 0.5, 0, 
    -0.5, -0.5, 0, 
    0.5, -0.5, 0 };

const float quad_vertices[] = { 
    0, 0.5, 0, 
    -0.5, -0.5, 0, 
    0.5, -0.5, 0,
    0.5, 0.5, 0
};

const float quad_normals[] = {
    1, 0, 0,
    0, 1, 0, 
    0, 0, 1,
    1, 1, 0
};

const float quad_uvs[] = {
    1, 0, 
    0, 1,  
    0.5, 0.5, 
    1, 1, 
};


const u16 quad_indices[] = {
    0,
    1,
    2,
    2,
    3,
    0
};

Mesh triangle_mesh = { .vertices = (f32 *)&triangle_vertices, .num_vertices = 3 };
Mesh quad_mesh = { .vertices = (f32 *) &quad_vertices, .normals = (f32 *) &quad_normals, .uvs = (f32 *) &quad_uvs, .indices = (u16 *) &quad_indices, .num_vertices = 4, .num_indices = 6 };

// END: Sketch renderer

// TODO: Since in WebAssembly we need to have async frame/main loop anyways, and tg_main will actually reutrn before
//       the first frame even starts, it is probably better to do the platform layer handmade-style: tg_init, tg_frame
//       and tg_shutdown.

void tgp_frame(tgp_Platform *p)
{
    tgp_pull(p);

    float r = (float)p->mouse.x / p->window.width;
    float g = (float)p->mouse.y / p->window.height;

    glClearColor(r, g, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mesh_draw(&quad_mesh);

    tgp_push(p);
}

char *vshader =
#ifdef __wasm__
    "#version 100\n"
#else
    "#version 130\n"
#endif
    "attribute vec3 pos;"
    "attribute vec3 normal;"
    "attribute vec2 uv;"
    "varying vec3 fs_normal;"
    "varying vec2 fs_uv;"
    "void main() {\n"
    "fs_normal = normal;"
    "fs_uv = uv;"
    "gl_Position = vec4(pos, 1);\n"
    "}";

char *fshader =
#ifdef __wasm__
    "#version 100\n"
    "precision mediump float;"
#else
    "#version 130\n"
#endif
    "varying vec3 fs_normal;"
    "varying vec2 fs_uv;"
    "void main() {\n"
    "gl_FragColor = vec4(fs_uv, 0, 1.0);\n"
    "}";


tinyobj_attrib_t attrib;

tgp_Platform p = { .window = { .width = 1024, .height = 768, .x = 100, .y = 100, .title = "Profuse-renderer" } };
int tg_main(int argc, char *argv[])
{
#ifdef __TG_RUN_TESTS
    tg_h_test();
    tg_math_test();
#endif

#if 0
    // p.user_context = (void *) &state;
    tgp_init(&p);

#ifdef _WIN32
    DEBUG_tgl_init(p.win32.window_handle);
#elif defined(__wasm__)
    DEBUG_tgl_init();
#endif

    GLuint program = DEBUG_tgl_create_program(vshader, fshader);
    glUseProgram(program);

#ifndef __wasm__
    u32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif
    glEnable(GL_CULL_FACE);

    tinyobj_parse_obj(&attrib,  bp)
    
    mesh_load(&quad_mesh);

    tgp_start_loop(&p);
#endif
    return 0;
}
