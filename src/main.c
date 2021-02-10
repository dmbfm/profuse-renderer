#include "common.h"
#include "heap.h"
#include "list.h"
#include "platform.h"
#include "platform_gl.h"
#include "renderer_gl.h"
#include "result.h"
#include "utils.h"

#ifdef __wasm__
Allocator *a = &heap_wasm_free_list_allocator;
#else
Allocator *a = &heap_allocator;
#endif

char *vshader =
#ifdef __wasm__
    "#version 100\n"
    "attribute vec3 pos;"
    "void main() {\n"
    "gl_Position = vec4(pos, 1);\n"
    "}";
#else
    "#version 330\n"
    "attribute vec3 pos;"
    "void main() {\n"
    "gl_Position = vec4(pos, 1);\n"
    "}";
#endif

char *fshader =
#ifdef __wasm__
    "#version 100\n"
    "void main() {\n"
    "gl_FragColor = vec4(1.0, 0, 0, 1.0);\n"
    "}";
#else
    "#version 330\n"
    "void main() {\n"
    "gl_FragColor = vec4(1.0f, 0, 0, 1.0f);\n"
    "}";
#endif

const f32 triangle_vertices[] = {-0.5, -0.5, 0, 0, 0.5, 0, 0.5, -0.5, 0};

Platform p_config(void) {
    Platform p = {0};

    return p;
}

#if defined(__wasm__)
typedef struct Asset {
    char    path[512];
    int     type;
    int     size;
    void *  data;
    boolean allocated;
    boolean isLoaded;
} Asset;

// Asset asset = { .path = "out.pfa" };
Asset assets[100];
int   asset_count;

int add_asset(const char *path) {
    Asset *a = &assets[asset_count];
    string_copy_len(&a->path[0], path, 512);

    asset_count++;
    return asset_count - 1;
}

Asset *get_asset(int id) {
    if (id < 0 || id >= asset_count) {
        return 0;
    }

    return &assets[id];
}

void read_file_cb(PlatformReadFileStatus status, usize byte_size, void *data, Allocator *allocator, void *user_data) {
    DEBUGLOG("Ok! Size = %d, id = %d", byte_size, *((int *)user_data));
    allocator->free(allocator, (uptr)data);
}

int  gid;
void server_start_loading_asset(int id) {
    Asset *asset = get_asset(id);
    gid          = id;
    DEBUGLOG("%s", asset->path);
    platform_read_file_async(a, asset->path, read_file_cb, &gid);
}

void test_load_asset() {
    int id = add_asset("out.pfa");
    server_start_loading_asset(id);
}
#endif

// "Client"
// client_asset_allocate(Asset *a);
// client_asset_loaded(Asset *a);

// "Server"
// server_start_loading_asset(Asset *asset);

void p_init(Platform *p) {
    UNUSED_VARIABLE(p);

    platform_print_line("p_init");

    Result(uptr) rx = a->alloc(a, sizeof(i32));

    i32 *x = (i32 *)result_unwrap(rx);

    *x = 10;
    platform_print_fmt(a, "x = %d", *x);

    DEBUGLOG("[main]: message");

    a->free(a, (uptr)x);

    Result(GLuint) rprogram = rgl_create_program_raw(a, vshader, fshader);
    GLuint program          = result_unwrap(rprogram);
    glUseProgram(program);

#ifndef __wasm__
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);

    test_load_asset();
}

void p_frame(Platform *p) {

    // DEBUGLOG("t = %f(s), dt = %f(s), %f(ms)\n", p->timing.time, p->timing.delta_s, p->timing.delta_ms);

    f32   r = (f32)p->mouse.x / p->window.width.value;
    f32   g = (f32)p->mouse.y / p->window.height.value;
    float b = (1.0f - r) * (1.0f - g);

    if (p->mouse.left_button.just_down) {
        r = 1;
        g = 1;
        b = 1;
    }

    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (p->window.was_resized) {
        glViewport(0, 0, p->window.width.value, p->window.height.value);
    }

    if (r > 0.5)
        p->window.cursor_style.value = PLATFORM_CURSOR_STYLE_HAND;
    else
        p->window.cursor_style.value = PLATFORM_CURSOR_STYLE_NORMAL;

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void p_shutdown(Platform *p) {
    platform_print_line("p_shutdown");
}
