#define TOOLBOX_DEBUG
#include "../src/toolbox/toolbox.h"
#include "../src/toolbox/unity_build.c"
#include <stdlib.h>

#define isalpha(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))
#define iseol(x)   ((x) == '\0' || (x) == '\n' || (x) == '\r')

//#define MODEL "bunny.obj"
//#define MODEL "teapot.obj"
//#define MODEL "head.obj"
//#define MODEL "erato-1.obj"
//#define MODEL "..\\test_models\\buddha.obj"
//#define MODEL "..\\test_models\\cornell_box.obj"
#define MODEL "..\\test_models\\sponza.obj"

typedef enum
{
    VERTEX,
    NORMAL,
    TEXTURE
} LineType;

typedef struct Parser
{
    LineType current_line;
} Parser;

typedef enum
{
    TOKEN_EOL,
    TOKEN_UNKOWN,
    TOKEN_NAME,
    TOKEN_VERTEX,
    TOKEN_NORMAL,
    TOKEN_TEXCOORD,
    TOKEN_FLOAT,
    TOKEN_FACE,
    TOKEN_INT,
    TOKEN_SLASH
} TokenKind;

typedef struct Token
{
    TokenKind kind;

    float float_value;
    int int_value;
} Token;

static Parser p;
char *stream;

Token token;
int current_line;

void next_token()
{
    char *start;
    char c;
repeat:
    start = stream;
    c     = *stream;
    switch (c) {
    case 0:
    case '\r':
    case '#':
    case '\n': {
        token.kind = TOKEN_EOL;
        stream++;
    } break;
    case ' ': {
        while (*stream == ' ')
            stream++;
        goto repeat;
    } break;

    case '/': {
        token.kind = TOKEN_SLASH;
        stream++;
    } break;

    case 'v': {
        stream++;
        char next = *stream++;
        switch (next) {
        case ' ': {
            token.kind = TOKEN_VERTEX;
        } break;

        case 't': {
            token.kind = TOKEN_TEXCOORD;
        } break;

        case 'n': {
            token.kind = TOKEN_NORMAL;
        } break;
        }
    } break;

    case 'f': {
        token.kind = TOKEN_FACE;
        stream++;
    } break;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z': {
        while (isalpha(*stream) || *stream == '.')
            stream++;
        token.kind = TOKEN_NAME;
    } break;

    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        char *s = stream;
        if (*stream == '-') {
            stream++;
        }
        while (t_isnum(*stream)) {
            stream++;
        }

        if (*stream == '.') {
            stream++;

            while (t_isnum(*stream)) {
                stream++;
            }

            if (*stream == 'e' || *stream == 'E')
            {
                stream++;
                if (*stream == '-') stream++;

                while (t_isnum(*stream))
                    stream++;
            }

            token.kind        = TOKEN_FLOAT;
            token.float_value = (float)strtod(s, 0);
        } else {
            token.kind      = TOKEN_INT;
            token.int_value = (int)strtol(s, 0, 10);
        }
    } break;
    default:
        token.kind = TOKEN_UNKOWN;
        stream++;
        break;
    }
}

void print_token()
{
    switch (token.kind) {
    case TOKEN_VERTEX:
        t_printf(" VERTEX ");
        break;

    case TOKEN_FACE:
        t_printf(" FACE ");
        break;

    case TOKEN_FLOAT:
        t_printf(" FLOAT(%f) ", token.float_value);
        break;

    case TOKEN_INT:
        t_printf(" INT(%d) ", token.int_value);
        break;

    case TOKEN_SLASH:
        t_printf(" SLASH(/) ");
        break;

    case TOKEN_NAME:
        t_printf(" NAME ");
        break;

    case TOKEN_EOL:
        t_printf(" EOL \n");
        break;

    default:
        t_printf(" TOKEN ");
        break;
    }
}

void assert_float()
{
    next_token();
    t_assert(token.kind == TOKEN_FLOAT);
}

void assert_float_or_int()
{
    next_token();
    t_assert(token.kind == TOKEN_FLOAT || token.kind == TOKEN_INT);
}

float assert_get_float_or_int()
{
    next_token();
    t_assert(token.kind == TOKEN_FLOAT || token.kind == TOKEN_INT);
    switch(token.kind)
    {
        case TOKEN_FLOAT: return token.float_value;
        case TOKEN_INT: return (float) token.int_value;
        default: return -1.0f;
    }
}

void assert_int()
{
    next_token();
    t_assert(token.kind == TOKEN_INT);
}

void assert_slash()
{
    next_token();
    t_assert(token.kind = TOKEN_SLASH);
}

void assert_eol()
{
    next_token();
    t_assert(token.kind == TOKEN_EOL);
}

Vec3 *vertices  = 0;
Vec3 *normals   = 0;
Vec2 *texcoords = 0;

typedef enum
{
    FACE_V,
    FACE_VN,
    FACE_VT,
    FACE_VTN,
    FACE_NOT_ASSERTED
} FaceType;

FaceType face_type = FACE_NOT_ASSERTED;

typedef struct FaceElement
{
    int v;
    int vn;
    int vt;

    u8 type;
} FaceElement;

typedef struct Face
{
    FaceElement faces[4];
    int count;
} Face;

Face *faces;

typedef enum
{
    FACE_VALUE_V,
    FACE_VALUE_N,
    FACE_VALUE_T,
} ProcessFaceType;

inline int process_face_value(int f, ProcessFaceType kind)
{
    if (f > 0) return f;

    switch(kind)
    {
        case FACE_VALUE_V:
            return t_arr_len(vertices) + f + 1;
        case FACE_VALUE_N:
            return t_arr_len(normals) + f + 1;
        case FACE_VALUE_T:
            return t_arr_len(texcoords) + f + 1;
    }
}

boolean parse_face_element(FaceElement *e)
{
    next_token();
    if (token.kind != TOKEN_INT)
        return false;

    e->v    = process_face_value(token.int_value, FACE_VALUE_V);
    e->type = FACE_V;

    if (*stream == '/') {
        stream++;

        if (*stream == '/') {
            stream++;
            assert_int();
            e->type = FACE_VN;
            e->vn   = process_face_value(token.int_value, FACE_VALUE_N);
        } else {
            assert_int();
            e->type = FACE_VT;
            e->vt   = process_face_value(token.int_value, FACE_VALUE_T);

            if (*stream == '/') {
                stream++;
                assert_int();
                e->type = FACE_VTN;
                e->vn   = process_face_value(token.int_value, FACE_VALUE_N);
            }
        }
    }

    return true;
}


void triangulate_face(Face *f, Face *out1, Face *out2)
{
    FaceElement e11, e12, e13, e21, e22, e23;

    e11 = f->faces[0];
    e12 = f->faces[1];
    e13 = f->faces[3];

    e21 = f->faces[3];
    e22 = f->faces[1];
    e23 = f->faces[2];

    *out1 = (Face) { .faces = { e11, e12, e13 }, .count = 3 };
    *out2 = (Face) { .faces = { e21, e22, e23 }, .count = 3 };
}

void parse_line(char *line)
{
    stream = line;

    next_token();
    // print_token();

    if (token.kind == TOKEN_VERTEX) {
        Vec3 v;

        v.x = assert_get_float_or_int();
        v.y = assert_get_float_or_int();
        v.z = assert_get_float_or_int();

        t_arr_push(vertices, v);

        assert_eol();
    }

    if (token.kind == TOKEN_NORMAL) {
        Vec3 v;

        v.x = assert_get_float_or_int();
        v.y = assert_get_float_or_int();
        v.z = assert_get_float_or_int();

        t_arr_push(normals, v);

        assert_eol();
    }

    if (token.kind == TOKEN_TEXCOORD) {
        Vec2 v;

        v.y = assert_get_float_or_int();
        v.y = assert_get_float_or_int();

        t_arr_push(texcoords, v);

        next_token();

        if (token.kind == TOKEN_FLOAT) {
            assert_eol();
        } else {
            t_assert(token.kind == TOKEN_EOL);
        }
    }

    if (token.kind == TOKEN_FACE) {
        Face f = { 0 };

        FaceElement e;
        while (parse_face_element(&e)) {
            f.faces[f.count] = e;
            f.count++;
        }

        if (f.count == 4)
        {
            Face f1, f2;
            triangulate_face(&f, &f1, &f2);
            t__arr_push(faces, f1);
            t__arr_push(faces, f2);
        }
        else
        {
            t_arr_push(faces, f);
        }

    }

    while (token.kind != TOKEN_EOL) {
        next_token();
        // print_token();
    }

    // t_printf("\n");
}


typedef struct
{
    u32 vertex_offset;
    u32 vertex_byte_size;
    u32 normal_offset;
    u32 normal_byte_size;
    u32 texcoord_offset;
    u32 texcoord_byte_size; 
} PFAHeader;

Vec3 *final_vertices;
Vec3 *final_normals;
Vec2 *final_texcoords;

void remove_indices()
{
    forn(i, t_arr_len(faces))
    {
        Face f = faces[i];

        t_assert(f.count == 3);

        forn(k, 3)
        {
            FaceElement e = f.faces[k];

            t_arr_push(final_vertices, vertices[e.v - 1]);

            if (e.type == FACE_VN || e.type == FACE_VTN)
            {
                t_arr_push(final_normals, normals[e.vn - 1]);
            }

            if (e.type == FACE_VT || e.type == FACE_VTN)
            {
                t_arr_push(final_texcoords, texcoords[e.vt - 1]);
            }
        }
    }
}


void build_normals()
{
    for (int i = 1; i <= t_arr_len(vertices); i++)
    {
        Vec3 sum = vec3(0, 0, 0);
        Vec3 v = vertices[i-1];
        forn(k, t_arr_len(faces))
        {
            int ia = 0, ib = 0;
            Face *f = &faces[k];

            if (f->faces[0].v == i)
            {
                ia = f->faces[1].v;
                ib = f->faces[2].v;
            } else if (f->faces[1].v == i) {
                ia = f->faces[2].v;
                ib = f->faces[0].v;
            } else if (f->faces[2].v == i) {
                ia = f->faces[0].v;
                ib = f->faces[1].v;
            } else {
                continue;
            }

            Vec3 a = vertices[ia-1];
            Vec3 b = vertices[ib-1];

            Vec3 e1 = vec3_sub(a, v);
            Vec3 e2 = vec3_sub(b, v);

            sum = vec3_add(sum, vec3_cross(e1, e2));

        }

        t_arr_push(normals, vec3_normalized(sum));
    }

    forn(k, t_arr_len(faces)) {
        Face *f = &faces[k];
        forn(kk, 3)
        {
            f->faces[kk].vn = f->faces[kk].v;

            if (f->faces[kk].type == FACE_VT)
                f->faces[kk].type = FACE_VTN;
            else if (f->faces[kk].type == FACE_V)
                f->faces[kk].type = FACE_VN;
        }
    }

    //for (int i = 0; i < t_arr_len(final_vertices); i += 3)
    //{
    //    Vec3 e1, e2, v1, v2, v3, normal;

    //    v1 = final_vertices[i];
    //    v2 = final_vertices[i+1];
    //    v3 = final_vertices[i+2];

    //    e1 = vec3_sub(v2, v1);
    //    e2 = vec3_sub(v3, v1);

    //    normal = vec3_normalized(vec3_cross(e1, e2));
    //    
    //    t_arr_push(final_normals, normal);
    //    t_arr_push(final_normals, normal);
    //    t_arr_push(final_normals, normal);
    //}
}

void output_binary()
{
    boolean built_normals = false;
    if (t_arr_len(normals) == 0)
    {
        build_normals();
        built_normals = true;
    }

    remove_indices();

//    if (t_arr_len(final_normals) == 0)
//        build_normals();

    FILE *bin = fopen("out.pfa", "wb");
    
    PFAHeader header = { 0 };
    u32 current_offest = sizeof(PFAHeader);

    fseek(bin, current_offest, SEEK_SET);
    if (t_arr_len(final_vertices) > 0)
    {
        header.vertex_offset = current_offest;
        header.vertex_byte_size = t_arr_len(final_vertices) * 3 * 4;
        current_offest += header.vertex_byte_size;

        forn(i, t_arr_len(final_vertices))
        {
            fwrite((void *)final_vertices[i].values, sizeof(f32), 3, bin);
        }
    }

    if (t_arr_len(final_normals) > 0)
    {
        header.normal_offset = current_offest;
        header.normal_byte_size = t_arr_len(final_normals) * 3 * 4;
        current_offest += header.normal_byte_size;

        forn(i, t_arr_len(final_normals))
        {
            fwrite((void *)final_normals[i].values, sizeof(f32), 3, bin);
        }
    }

    if (t_arr_len(final_texcoords) > 0)
    {
        header.texcoord_offset = current_offest;
        header.texcoord_byte_size = t_arr_len(final_texcoords) * 2 * 4;
        current_offest += header.texcoord_byte_size;

        forn(i, t_arr_len(final_texcoords))
        {
            fwrite((void *)final_texcoords[i].values, sizeof(f32), 2, bin);
        }
    }

    fseek(bin, 0, SEEK_SET);
    fwrite((void *) &header.vertex_offset, sizeof(u32), 1, bin);
    fwrite((void *) &header.vertex_byte_size, sizeof(u32), 1, bin);
    fwrite((void *) &header.normal_offset, sizeof(u32), 1, bin);
    fwrite((void *) &header.normal_byte_size, sizeof(u32), 1, bin);
    fwrite((void *) &header.texcoord_offset, sizeof(u32), 1, bin);
    fwrite((void *) &header.texcoord_byte_size, sizeof(u32), 1, bin);

    t_printf("%d vertices, %d tex coords.\n", t_arr_len(final_vertices), t_arr_len(final_texcoords));
}

int main(int argc, char **argv)
{
    t_printf("obj test \n");

    int nlines;
    char **lines;

    lines = t_read_file_lines(MODEL, &nlines);

    forn(i, nlines)
    {
        current_line = i;
        if (current_line == 125)
        {
            current_line = 0;
        }
        parse_line(lines[i]);
    }

    //forn(i, t_arr_len(vertices))
    //{
    //    Vec3 v = vertices[i];
    //    t_printf("V(%d): %f, %f, %f\n", i, v.x, v.y, v.z);
    //}
    //forn(i, t_arr_len(normals))
    //{
    //    Vec3 v = normals[i];
    //    t_printf("N(%d): %f, %f, %f\n", i, v.x, v.y, v.z);
    //}

    //forn(i, t_arr_len(texcoords))
    //{
    //    Vec2 v = texcoords[i];
    //    t_printf("T(%d): %f, %f\n", i, v.x, v.y);
    //}

    //forn(i, t_arr_len(faces))
    //{
    //    Face f = faces[i];
    //    t_printf("F(%d): ", i);
    //    forn(k, f.count)
    //    {
    //        FaceElement e = f.faces[k];
    //        switch (e.type) {
    //        case FACE_V:

    //            t_printf(" %d ", e.v);
    //            break;
    //        case FACE_VT:

    //            t_printf(" %d/%d ", e.v, e.vt);
    //            break;
    //        case FACE_VN:
    //            t_printf(" %d//%d ", e.v, e.vn);
    //            break;
    //        case FACE_VTN:
    //            t_printf(" %d/%d/%d ", e.v, e.vt, e.vn);
    //            break;
    //        }
    //    }
    //    t_printf("\n");
    //}

    FILE *file = fopen("out.obj", "w");

    t_assert(file);

    forn(i, t_arr_len(vertices))
    {
        Vec3 v = vertices[i];
        fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
    }
    forn(i, t_arr_len(normals))
    {
        Vec3 v = normals[i];
        fprintf(file, "vn %f %f %f\n", v.x, v.y, v.z);
    }

    forn(i, t_arr_len(texcoords))
    {
        Vec2 v = texcoords[i];
        fprintf(file, "vt %f %f\n", v.x, v.y);
    }

    forn(i, t_arr_len(faces))
    {
        Face f = faces[i];
        fprintf(file, "f ");
        forn(k, f.count)
        {
            FaceElement e = f.faces[k];
            switch (e.type) {
            case FACE_V:

                fprintf(file, " %d ", e.v);
                break;
            case FACE_VT:

                fprintf(file, " %d/%d ", e.v, e.vt);
                break;
            case FACE_VN:
                fprintf(file, " %d//%d ", e.v, e.vn);
                break;
            case FACE_VTN:
                fprintf(file, " %d/%d/%d ", e.v, e.vt, e.vn);
                break;
            }
        }
        fprintf(file, "\n");
    }

    output_binary();

    return 0;
}
