#include "coffee.h"
#include "toolbox.h"

#include "renderer_gl.c"

typedef struct
{
    Vec3 position;
    Vec3 scale;
} Transform;

typedef struct
{
    u32 gltexture;
} Texture;

typedef struct
{
    char *material_name;
    Texture diffuse_texture;
    i32 diffuse_image_id;
    i32 material_id;
    boolean fully_loaded;
} Material;

#define MAX_MATERIALS 256
Material materials[MAX_MATERIALS];
u32 num_materials;

typedef struct
{
    int num_vertices;
    int num_triangles;
    f32 *vertices;
    f32 *normals;
    f32 *tex_coords;
    f32 *tex_coords2;
    u16 *indices;


    // temp stuff
    i32 material_id;
    uint vbo;
    uint ebo;
    Vec3 color;
} Mesh;

typedef struct
{
    Mesh mesh;
    Transform transform;
} Entity;

typedef struct TransformHierarchyNode
{
    Transform *t;
    Entity *e;
    struct TransformHierarchyNode **children;
    struct TransformHierarchyNode *parent;
} TransformHierarchyNode;

typedef struct Camera
{
    float n;
    float f;
    float fov;
    float aspect;

    Vec3 position;
    Vec3 target;

    Mat4 projection_matrix;
    Mat4 view_matrix;
} Camera;

extern f32 rand();
// clang-format off
WASM_JS(
        function rand() {
            return Math.random();
        }
)
// clang-format on

void mesh_load(Mesh *mesh)
{
    t_assert(mesh->vertices);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

    int buffer_size = 3 * 4 * mesh->num_vertices;

    if (mesh->normals)
        buffer_size *= 2;

    if (mesh->tex_coords)
        buffer_size += 8 * mesh->num_vertices;

    glBufferData(GL_ARRAY_BUFFER, buffer_size, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 4 * mesh->num_vertices, (const void *)mesh->vertices);

    int offset = 12 * mesh->num_vertices;

    if (mesh->normals) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, 12 * mesh->num_vertices, (const void *)mesh->normals);
        offset *= 2;
    }

    if (mesh->tex_coords) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, 8 * mesh->num_vertices, (const void *)mesh->tex_coords);
        offset += 8 * mesh->num_vertices;
    }

    if (mesh->indices) {
        glGenBuffers(1, &mesh->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * mesh->num_triangles, (const void *)mesh->indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh_draw(Mesh *mesh)
{
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);

    int idx    = 1;
    u64 offset = 12 * mesh->num_vertices;

    if (mesh->normals) {
        glVertexAttribPointer(idx, 3, GL_FLOAT, 0, 0, (const void *)offset);
        glEnableVertexAttribArray(idx);
        offset *= 2;
        idx++;
    }

    if (mesh->tex_coords) {
        glVertexAttribPointer(idx, 2, GL_FLOAT, 0, 0, (const void *)offset);
        glEnableVertexAttribArray(idx);
        offset += 8 * mesh->num_vertices;
        idx++;
    }

    if (!mesh->indices) {
        glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
    } else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        // TODO: Check for extension that allows INT indices
        glDrawElements(GL_TRIANGLES, 3 * mesh->num_triangles, GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
}

char *vshader =
#ifdef __wasm__
    "#version 100\n"
    "precision mediump float;"
#else
    "#version 130\n"
#endif
    "attribute vec3 pos;"
    "attribute vec3 normal;"
    "attribute vec2 uv;"
    "uniform vec3 u_color;"
    "uniform vec3 u_camera_position;"
    "uniform mat4 modelTransform;"
    "uniform mat4 u_view_matrx;"
    "uniform mat4 u_proj_matrix;"
    "varying vec3 fs_normal;"
    "varying vec3 fs_normal_view;"
    "varying vec2 fs_uv;"
    "varying vec3 fs_pos;"
    "void main() {\n"
    "fs_normal = (modelTransform * vec4(normalize(normal), 0)).xyz;"
    "fs_normal_view = (u_view_matrx * vec4(fs_normal, 0)).xyz;"
    "fs_uv = uv;"
    "gl_Position = u_proj_matrix * u_view_matrx * modelTransform * vec4(pos, 1);\n"
    "fs_pos = (modelTransform * vec4(pos, 1)).xyz;"
    "}";

char *fshader =
#ifdef __wasm__
    "#version 100\n"
    "precision mediump float;"
#else
    "#version 130\n"
#endif
    "varying vec3 fs_normal_view;"
    "varying vec3 fs_normal;"
    "varying vec2 fs_uv;"
    "varying vec3 fs_pos;"
    "uniform vec3 u_camera_position;"
    "uniform vec3 u_color;"
    "void main() {\n"
    "vec3 lpos = vec3(10, 10, -10);"
    "vec3 ldir = normalize(lpos - fs_pos);"
    // 242, 176, 114
    //"vec3 scolor = vec3(242.0 / 255.0, 176.0 / 255.0, 114.0 / 255.0);"
    //"vec3 scolor = vec3(155.0 / 255.0, 152.0 / 255.0, 212.0 / 255.0);"
    "vec3 scolor = u_color;"
    "vec3 c = 0.05 * scolor * max(0.0, dot(lpos, normalize(fs_normal)));"
    "c += scolor * 0.01;"
    "vec3 eye = normalize(u_camera_position - fs_pos);"
    "vec3 n = normalize(fs_normal);"
    "vec3 reflight = -normalize((ldir - 2.0 * dot(ldir, n) * n));"
    "c += pow(clamp(0.98 * dot(reflight, eye), 0.0, 1.0), 70.0);"
    "c = pow(clamp(c, 0.0, 1.0), vec3(1.0 / 2.2));"
    "gl_FragColor = vec4(normalize(fs_normal_view), 1.0);\n"
    //"gl_FragColor = vec4(fs_uv, 0, 1.0);\n"
    //"gl_FragColor = vec4(c, 1.0);\n"
    "}";

const float triangle_vertices[] = { 0, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0 };

const float quad_vertices[] = { 0, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0 };

const float quad_normals[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0 };

const float quad_uvs[] = {
    1, 0, 0, 1, 0.5, 0.5, 1, 1,
};

Mesh mesh = { .vertices = (f32 *)&triangle_vertices, .num_triangles = 1, .num_vertices = 3 };
GLuint program;
int model_matrix_loc    = -1;
int view_matrix_loc     = -1;
int proj_matrix_loc     = -1;
int camera_position_loc = -1;
int color_loc           = -1;
Camera camera;

// clang-format off
extern void fetch_image(char *baseUrl, char *url, u32 material_id, void on_load(u32, u32));
WASM_JS(
        let imagesPool = new ObjectPool();
)
WASM_JS(
        function fetch_image(baseUrlPtr, urlPtr, materialId, cb) {
            let f = table.get(cb);
            let url = decodeStringAt(urlPtr);
            if (baseUrlPtr) {
                let baseUrl = decodeStringAt(baseUrlPtr);
                url = baseUrl + "/" + url;
            }
            let img = new Image();
            document.body.appendChild(img);
            let id = imagesPool.add(img);
            img.onload = () => { console.log("fetch_image", materialId, id); f(materialId, id);  };
            img.src = url;

        }
)

extern void fetch_binary_file_async(char *url, void cb(u8 *result));
WASM_JS(
        function fetch_binary_file_async(urlPtr, cb) {
            let f = table.get(cb);
            if (!f) {
                console.log("[fetch_binary_file_async]: Invalid callback!");
            }

            let url = decodeStringAt(urlPtr);
            fetch(url).then(x => x.arrayBuffer()).then(data => {
                    let byteSize = data.byteLength;
                    let buffer = malloc(byteSize); 

                    let dataU8 = new Uint8Array(data);
                    let i = byteSize;
                    while(i) {
                        u8[buffer+i-1] = dataU8[i-1];
                        i--;
                    }

                    f(buffer);
            });
        }
)

extern void callback_test(void cb(u8 *result), char *buffer);
WASM_JS(
        function callback_test(cb, ptr) {
            console.log("cb", cb);
            console.log(table);
            let f = table.get(cb);
            fetch("/tools/out.pfa").then(x => x.arrayBuffer()).then(data => { 
                    let byteSize = data.byteLength;
                    let s = malloc(byteSize);

                    let dataU8 = new Uint8Array(data);

                    let i = byteSize;
                    while(i)
                    {
                        u8[s + i - 1] = dataU8[i - 1]; 
                        i--;
                    }

                    let dataF32 = new Float32Array(data);

                    f(s); 
                    })
        }
)

// clang-format on

Mesh teapot;
boolean teapot_loaded;

Mesh meshes[256];
u32 num_meshes;

typedef struct
{
    u8 *start;
    u8 *head;
    u8 *save_point;
} DataStream;

u32 data_stream_read_u32(DataStream *stream)
{
    u32 v = *((u32 *)stream->head);
    stream->head += sizeof(u32);

    return v;
}

u8 data_stream_read_u8(DataStream *stream)
{
    u8 v = *stream->head;
    stream->head++;

    return v;
}

void data_stream_set_pos(DataStream *stream, u32 pos)
{
    stream->head = stream->start + pos;
}

u32 data_stream_get_pos(DataStream *stream)
{
    return (stream->head - stream->start);
}

void data_stream_save_point(DataStream *stream)
{
    stream->save_point = stream->head;
}

void data_stream_restore_save_point(DataStream *stream)
{
    stream->head = stream->save_point;
}

u32 data_stream_read_string(DataStream *stream, char *out, u32 max_byte_length)
{
    char *s = out;
    u32 bytes_read = 0;
    u32 len = max_byte_length;

    if (max_byte_length == 0) {
        return 0;
    } else if (max_byte_length == 1) {
        *out = 0;
        return 1;
    }

    while (*stream->head && len > 1)
    {
        *s++ = data_stream_read_u8(stream);
        len--;
        bytes_read++;
    }

    stream->head++;
    bytes_read++;
    *s = 0; 

    return bytes_read;
}

boolean data_stream_read_serialized_string(DataStream *stream, u32 max_byte_length, char *out, u32 *out_bytes_read)
{
    // Strings are serialized by placing a u32 indicating byte length, followed by the null-terminated string.
    u32 serialized_string_byte_length = data_stream_read_u32(stream);

    // If the byte-length is zero, nothing to do. 
    if (serialized_string_byte_length == 0) {
        *out_bytes_read = 0;
        return true;
    }

    // NOTE: If the string does not fit in the buffer, don't read the string, and return -1 to indicate that.
    if (max_byte_length < serialized_string_byte_length) {
        *out_bytes_read = 0;
        return false;
    }

    *out_bytes_read = data_stream_read_string(stream, out, max_byte_length);

    return true;
}

void image_fetch_callback(u32 mat_id, u32 tex_id)
{
    t_printf("[image_fetch_callback]: mat = %d, image = %d\n", mat_id, tex_id);

    Material *mat = (materials + mat_id - 1);
    mat->diffuse_image_id = tex_id;
    mat->fully_loaded = true;
}

void matlib_fetch_callback(u8* data)
{
    DataStream ds = { .head = data, .start = data };

    u32 _num_materials = data_stream_read_u32(&ds);

    t_printf("num_materials: %d\n", num_materials);

    data_stream_save_point(&ds);
    forn(i, _num_materials) {
        data_stream_restore_save_point(&ds);
        u32 material_offset = data_stream_read_u32(&ds);
        data_stream_save_point(&ds);

        t_printf("material %d offset: %d\n", i, material_offset);

        data_stream_set_pos(&ds, material_offset);

        u32 material_name_length;
        char material_name[256];
        if ((data_stream_read_serialized_string(&ds, 256, material_name, &material_name_length)) && (material_name_length > 0)) {
            t_printf("material_name(%d): %s\n", material_name_length, material_name);
        }

        u32 diffuse_texture_length;
        char diffuse_texture[256];
        if ((data_stream_read_serialized_string(&ds, 256, diffuse_texture, &diffuse_texture_length)) && (diffuse_texture_length > 0)) {
            t_printf("diffuse_texture(%d): %s\n", diffuse_texture_length, diffuse_texture);
            fetch_image("/test_models", diffuse_texture, i + 1, image_fetch_callback);
        }

        //t_printf("MAT INDEX %d\n", i);
        Material mat = { .material_id = i + 1, .material_name = material_name };
        materials[i] = mat;
        num_materials++;
    }
}

void cbex(u8 *s)
{
    DataStream ds = { .head = s, .start = s };

    num_meshes = data_stream_read_u32(&ds);

    u32 pos = data_stream_get_pos(&ds);
    forn(i, num_meshes)
    {
        data_stream_set_pos(&ds, pos);
        u32 mesh_offset = data_stream_read_u32(&ds);
        pos             = data_stream_get_pos(&ds);

        data_stream_set_pos(&ds, mesh_offset);

        u32 vertex_offset      = data_stream_read_u32(&ds);
        u32 vertex_byte_size   = data_stream_read_u32(&ds);
        u32 normal_offset      = data_stream_read_u32(&ds);
        u32 normal_byte_size   = data_stream_read_u32(&ds);
        u32 texcoord_offset    = data_stream_read_u32(&ds);
        u32 texcoord_byte_size = data_stream_read_u32(&ds);
        u32 material_index     = data_stream_read_u32(&ds);

        mesh.vertices      = (f32 *)(ds.start + vertex_offset);
        mesh.num_vertices  = (vertex_byte_size / 12);
        mesh.num_triangles = mesh.num_vertices / 3;

        if (normal_offset > 0) {
            mesh.normals = (f32 *)(ds.start + normal_offset);
        }

        if (texcoord_offset > 0) {
            mesh.tex_coords = (f32 *)(ds.start + texcoord_offset);
        }

        mesh.color = vec3(rand(), rand(), rand());
        mesh.material_id = material_index;

        mesh_load(&mesh);
        meshes[i] = mesh;

        t_printf("material_index(%d): %d\n", i, material_index);
    }

    teapot_loaded = true;
    t_free(s);
}

static Coffee c;
int coffee_main(int argc, char **argv)
{
    t_printf("Fucksz\n");

    fetch_binary_file_async("/tools/out.pfa", cbex);
    fetch_binary_file_async("/tools/out.pml", matlib_fetch_callback);

    c.mouse.cursor_style = COFFEE_CURSOR_STYLE_NORMAL;

    coffee_init(&c);

    rgl_create_program_vf(vshader, fshader, &program);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    mesh_load(&mesh);
    glUseProgram(program);

    coffee_run(&c);

    model_matrix_loc    = glGetUniformLocation(program, "modelTransform");
    view_matrix_loc     = glGetUniformLocation(program, "u_view_matrx");
    proj_matrix_loc     = glGetUniformLocation(program, "u_proj_matrix");
    camera_position_loc = glGetUniformLocation(program, "u_camera_position");
    color_loc           = glGetUniformLocation(program, "u_color");

    camera = (Camera){ .n        = 0.1,
                       .f        = 1000,
                       .aspect   = (f32)c.window.width / c.window.height,
                       .fov      = 50,
                       .position = vec3(2, 2, 2),
                       .target   = vec3(0, 0, 0) };

    return 0;
}

f32 x       = 0;
float angle = 0;
float polar = T_PI / 2;
float azimuthal;
float radius      = 4;
float orbit_speed = 10.0f;
void coffee_frame(Coffee *c)
{
    coffee_pull(c);

    float r = (float)c->mouse.x / c->window.width;
    float g = (float)c->mouse.y / c->window.height;

    angle += 0.01f;

    if (c->mouse.left_button.is_down) {
        azimuthal += orbit_speed * c->mouse.delta_x / c->window.width;

        polar -= orbit_speed * c->mouse.delta_y / c->window.height;
        polar = t_clamp(polar, 0.0001f, T_PI - 0.0001f);
    }

    radius += 0.001f * c->mouse.mouse_wheel_delta;

    camera.position = vec3_spherical(polar, azimuthal, radius);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.view_matrix       = mat4_look_at(camera.position, camera.target, vec3_j());
    camera.projection_matrix = mat4_perspective(T_DEG2RAD(camera.fov), camera.aspect, camera.n, camera.f);

    x += 0.001f;
    Mat4 t = mat4_translation(vec3(4.0f * (r - 0.5f), -4.0f * (g - 0.5f), 0));
    glUniformMatrix4fv(model_matrix_loc, 1, 0, (const GLfloat *)&t.values[0]);
    glUniformMatrix4fv(view_matrix_loc, 1, 0, (const GLfloat *)&camera.view_matrix.values[0]);
    glUniformMatrix4fv(proj_matrix_loc, 1, 0, (const GLfloat *)&camera.projection_matrix.values[0]);
    glUniform3fv(camera_position_loc, (const GLfloat *)&camera.position.values[0]);

    if (teapot_loaded) {
        t     = mat4_id();
        t.m11 = t.m22 = t.m33 = 0.015;
        // t = mat4_mul(mat4_rotY(angle), t);
        glUniformMatrix4fv(model_matrix_loc, 1, 0, (const GLfloat *)&t.values[0]);
        forn(i, num_meshes)
        {
            glUniform3fv(color_loc, (const GLfloat *)&meshes[i].color.values[0]);
            mesh_draw(&meshes[i]);
        }
    }

    coffee_push(c);
}

void coffee_shutdown(Coffee *c) {}
