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
    int num_vertices;
    int num_triangles;
    f32 *vertices;
    f32 *normals;
    f32 *tex_coords;
    f32 *tex_coords2;
    u16 *indices;

    // temp stuff
    uint vbo;
    uint ebo;
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
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 4 * mesh->num_vertices, (const void *) mesh->vertices);

    int offset = 12 * mesh->num_vertices;

    if (mesh->normals)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset, 12 * mesh->num_vertices, (const void *) mesh->normals);
        offset *= 2;
    }

    if (mesh->tex_coords)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset, 8 * mesh->num_vertices, (const void *) mesh->tex_coords);
        offset += 8 * mesh->num_vertices;
    }

    if (mesh->indices)
    {
        glGenBuffers(1, &mesh->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * mesh->num_triangles, (const void *) mesh->indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh_draw(Mesh *mesh)
{
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(0);

        int idx = 1;
        u64 offset = 12 * mesh->num_vertices;

        if (mesh->normals) 
        {
            glVertexAttribPointer(idx, 3, GL_FLOAT, 0, 0, (const void *) offset);
            glEnableVertexAttribArray(idx);
            offset *= 2;
            idx++;
        }

        if (mesh->tex_coords) 
        {
            glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, (const void *) offset);
            glEnableVertexAttribArray(2);
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
    "uniform vec3 u_camera_position;"
    "uniform mat4 modelTransform;"
    "uniform mat4 u_view_matrx;"
    "uniform mat4 u_proj_matrix;"
    "varying vec3 fs_normal;"
    "varying vec2 fs_uv;"
    "varying vec3 fs_pos;"
    "void main() {\n"
    "fs_normal = (modelTransform * vec4(normalize(normal), 0)).xyz;"
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
    "varying vec3 fs_normal;"
    "varying vec2 fs_uv;"
    "varying vec3 fs_pos;"
    "uniform vec3 u_camera_position;"
    "void main() {\n"
    "vec3 lpos = vec3(10, 10, -10);"
    "vec3 ldir = normalize(lpos - fs_pos);"
    // 242, 176, 114
    //"vec3 scolor = vec3(242.0 / 255.0, 176.0 / 255.0, 114.0 / 255.0);"
    "vec3 scolor = vec3(155.0 / 255.0, 152.0 / 255.0, 212.0 / 255.0);"
    "vec3 c = 0.05 * scolor * max(0.0, dot(lpos, normalize(fs_normal)));"
    "c += scolor * 0.01;"
    "vec3 eye = normalize(u_camera_position - fs_pos);"
    "vec3 n = normalize(fs_normal);"
    "vec3 reflight = -normalize((ldir - 2.0 * dot(ldir, n) * n));"
    "c += pow(clamp(0.98 * dot(reflight, eye), 0.0, 1.0), 70.0);"
    "c = pow(clamp(c, 0.0, 1.0), vec3(1.0 / 2.2));" 
    "gl_FragColor = vec4(c, 1.0);\n"
    "}";


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

Mesh mesh = { .vertices = (f32 *)&triangle_vertices, .num_triangles = 1, .num_vertices = 3 };
GLuint program;
int model_matrix_loc = -1;
int view_matrix_loc = -1;
int proj_matrix_loc = -1;
int camera_position_loc = -1;
Camera camera;

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

Mesh teapot;
boolean teapot_loaded;
void cbex(u8 *s) {
    u8 *data = (u8 *)s;
    u32 *data32 = (u32 *) s;
    u32 vertex_offset = *(data32);
    u32 vertex_byte_size = *(data32 + 1);
    u32 normal_offset = *(data32 + 2);
    u32 normal_byte_size = *(data32 + 3);
    u32 texcoord_offset = *(data32 + 4);
    u32 texcoord_byte_size = *(data32 + 5);

   // TODO: BUG printf uses malloc which was causing corruption in the malloc'd data for the teapot buffers!
   //
   // t_printf("odfsdfasdfasdf");
   //t_printf("vertex_offset = %d\n", vertex_offset);
   //t_printf("vertex_byte_size = %d\n", vertex_byte_size);
   //t_printf("normal_offset = %d\n", normal_offset);
   //t_printf("normal_byte_size = %d\n", normal_byte_size);

   teapot.vertices = (f32 *) ((u8*)data + vertex_offset);

   if (texcoord_offset != 0)
   {
       teapot.tex_coords = (f32 *) ((u8*)data + texcoord_offset);
   }

   if (normal_offset != 0)
   {
       teapot.normals = (f32 *) (data + normal_offset);
   }

   teapot.num_vertices = (vertex_byte_size / 12);
   teapot.num_triangles = teapot.num_vertices / 3;

   mesh_load(&teapot);
   teapot_loaded = true;

   t_free(s);
}

int coffee_main(int argc, char **argv)
{
    t_printf("Fucksz\n");

    char buf[512];
    callback_test(cbex, buf);

    Coffee c = { 0 };
    c.mouse.cursor_style = COFFEE_CURSOR_STYLE_NORMAL;

    coffee_init(&c);

    rgl_create_program_vf(vshader, fshader, &program);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    mesh_load(&mesh);
    glUseProgram(program);
    //mesh_draw(&mesh);

    coffee_run(&c);

    model_matrix_loc = glGetUniformLocation(program, "modelTransform");
    view_matrix_loc = glGetUniformLocation(program, "u_view_matrx");
    proj_matrix_loc = glGetUniformLocation(program, "u_proj_matrix");
    camera_position_loc = glGetUniformLocation(program, "u_camera_position");


    camera = (Camera) { .n = 0.1, .f = 1000, .aspect = (f32) c.window.width / c.window.height, .fov = 50, .position = vec3(2, 2, 2), .target = vec3(0, 0, 0) };

    return 0;
}

f32 x = 0;
float angle = 0;
float polar = T_PI / 2;
float azimuthal;
float radius = 4;
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

    camera.view_matrix = mat4_look_at(camera.position, camera.target, vec3_j());
    camera.projection_matrix = mat4_perspective(T_DEG2RAD(camera.fov), camera.aspect, camera.n, camera.f);

    x += 0.001f;
    Mat4 t = mat4_translation(vec3(4.0f * (r - 0.5f), -4.0f * (g - 0.5f), 0));
    glUniformMatrix4fv(model_matrix_loc, 1, 0, (const GLfloat *) &t.values[0]);
    glUniformMatrix4fv(view_matrix_loc, 1, 0, (const GLfloat *) &camera.view_matrix.values[0]);
    glUniformMatrix4fv(proj_matrix_loc, 1, 0, (const GLfloat *) &camera.projection_matrix.values[0]);
    glUniform3fv(camera_position_loc, (const GLfloat *) &camera.position.values[0]);

    //mesh_draw(&mesh);

    if (teapot_loaded)
    {
        t = mat4_id();
        t.m11 = t.m22 = t.m33 = 0.015;
        //t = mat4_mul(mat4_rotY(angle), t);
        glUniformMatrix4fv(model_matrix_loc, 1, 0, (const GLfloat *) &t.values[0]);
        mesh_draw(&teapot);
    }

    coffee_push(c);

}

void coffee_shutdown(Coffee *c)
{

}
