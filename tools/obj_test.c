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

boolean get_base_dir(char *filepath, char *out, u32 max_len)
{
    char *start      = filepath;
    char *last_slash = start;
    while (*start) {
        if (*start == '\\' || *start == '/') {
            last_slash = start;
        }

        start++;
    }

    if ((last_slash - start) >= max_len)
        return false;

    start = filepath;
    while ((last_slash - start) >= 0) {
        *out++ = *start;
        start++;
    }
    *out = 0;

    return true;
}

typedef struct StringInternEntry
{
    size_t len;
    char *str;
} StringInternEntry;

StringInternEntry *string_intern_entries;

const char *string_intern_range(char *start, char *end)
{
    size_t len = end - start;
    forn(i, t_arr_len(string_intern_entries))
    {
        StringInternEntry e = string_intern_entries[i];

        if (e.len == len && strncmp(start, e.str, e.len) == 0) {
            return e.str;
        }
    }

    char *str = (char *)malloc(len + 1);
    memcpy(str, start, len);
    str[len] = 0;

    StringInternEntry e = { .str = str, .len = len };

    t_arr_push(string_intern_entries, e);

    return str;
}

const char *string_intern(char *str)
{
    return string_intern_range(str, str + strlen(str));
}

#define MAX_KEYWORDS 128
const char *keywords[MAX_KEYWORDS];
typedef enum
{
    KEYWORD_VERTEX = 0,
    KEYWORD_NORMAL = 1,
    KEYWORD_TEXCOORD,
    KEYWORD_FACE,
    KEYWORD_GROUP,
    KEYWORD_USE_MTL,
    KEYWORD_SMOOTH,
    KEYWORD_OBJECT,
    KEYWORD_MTLLIB,
    KEYWORD_OFF,
    KEYWORD_NEWMTL,
    KEYWORD_MAP_KD
} Keywords;

boolean is_keyword(const char *name)
{
    const char *k = 0;
    int i         = 0;

    while ((k = keywords[i])) {
        if (name == k)
            return true;
        i++;
    }

    return false;
}

typedef enum
{
    VERTEX,
    NORMAL,
    TEXTURE
} LineType;

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

typedef struct Object
{
    Vec3 *output_vertices;
    Vec3 *output_normals;
    Vec2 *output_texcoords;

    Face *faces;
    const char *material_name;
} Object;

enum
{
    MAX_OBJECTS = 4096
};

typedef struct Parser
{
    Vec3 *vertices;
    Vec3 *normals;
    Vec2 *texcoords;

    Object objects[MAX_OBJECTS];

    int num_objects;
    int current_object;
    int current_line;
} Parser;

static Parser parser = { 0 };

void parser_init()
{
    parser.num_objects    = 1;
    parser.current_object = 0;
}

void push_face(Face f)
{
    t_arr_push(parser.objects[parser.current_object].faces, f);
}

Object *parser_get_current_object()
{
    return &parser.objects[parser.current_object];
}

void parser_set_material(const char *material_name)
{
    Object *o = parser_get_current_object();

    if (!o->material_name) {
        o->material_name = material_name;
    } else {
        forn(i, parser.num_objects)
        {
            if (parser.objects[i].material_name == material_name) {
                parser.current_object = i;
                return;
            }
        }

        parser.current_object                               = parser.num_objects;
        parser.objects[parser.current_object].material_name = material_name;
        parser.num_objects++;
    }
}

typedef enum
{
    TOKEN_EOL = 0,
    // Trick from Per Vognsen
    TOKEN_UNKOWN = 127,
    TOKEN_NAME,
    TOKEN_KEYWORD,
    TOKEN_FLOAT,
    TOKEN_INT,
} TokenKind;

const char *token_kind_str[] = {
    [TOKEN_EOL] = "[EOL]",         [TOKEN_UNKOWN] = "[Unkown]", [TOKEN_NAME] = "[Name]",
    [TOKEN_KEYWORD] = "[Keyword]", [TOKEN_INT] = "[Int]",       [TOKEN_FLOAT] = "[Float]"
};

typedef struct Token
{
    TokenKind kind;

    float float_value;
    int int_value;
    const char *string_value;
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
    case '#':
    case '\r':
    case '\n': {
        token.kind = TOKEN_EOL;
        stream++;
    } break;
    case '\t':
    case ' ': {
        while (*stream == ' ' || *stream == '\t')
            stream++;
        goto repeat;
    } break;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
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
    case 'v':
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
        while (isalpha(*stream) || t_isnum(*stream) || *stream == '.' || *stream == '_' || *stream == '\\')
            stream++;
        token.string_value = string_intern_range(start, stream);
        token.kind         = is_keyword(token.string_value) ? TOKEN_KEYWORD : TOKEN_NAME;
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

            if (*stream == 'e' || *stream == 'E') {
                stream++;
                if (*stream == '-')
                    stream++;

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
        token.kind = *stream++;
        break;
    }
}

boolean match_keyword(const char *keyword)
{
    if ((token.kind == TOKEN_KEYWORD) && (token.string_value == keyword)) {
        next_token();
        return true;
    }

    return false;
}

boolean match_token(TokenKind kind)
{
    if (token.kind == kind) {
        next_token();
        return true;
    }

    return false;
}

void print_token();
void expect_token(TokenKind kind)
{
    if (token.kind != kind) {
        t_printf("Invalid token!\n");
        print_token();
        abort();
    }

    next_token();
}

boolean is_token(TokenKind kind)
{
    return token.kind == kind;
}

float read_number()
{
    float result;

    if (is_token(TOKEN_FLOAT)) {
        result = token.float_value;
    } else if (is_token(TOKEN_INT)) {
        result = (float)token.int_value;
    } else {
        t_printf("Number expected!\n");
        abort();
    }

    next_token();
    return result;
}

void print_token()
{
    if (token.kind > 0 && token.kind < 127) {
        t_printf("[%c(%i)]", token.kind, token.kind);
        return;
    }

    switch (token.kind) {
    case TOKEN_KEYWORD:
        t_printf(" {%s} ", token.string_value);
        break;

    case TOKEN_FLOAT:
        t_printf(" [%ff] ", token.float_value);
        break;

    case TOKEN_INT:
        t_printf(" [%d] ", token.int_value);
        break;

    case TOKEN_NAME:
        t_printf(" (%s) ", token.string_value);
        break;

    case TOKEN_EOL:
        t_printf(" EOL \n");
        break;

    default:
        break;
    }
}

typedef enum
{
    FACE_VALUE_V,
    FACE_VALUE_N,
    FACE_VALUE_T,
} ProcessFaceType;

inline int process_face_value(int f, ProcessFaceType kind)
{
    if (f > 0)
        return f;

    switch (kind) {
    case FACE_VALUE_V:
        return t_arr_len(parser.vertices) + f + 1;
    case FACE_VALUE_N:
        return t_arr_len(parser.normals) + f + 1;
    case FACE_VALUE_T:
        return t_arr_len(parser.texcoords) + f + 1;
    }
}

boolean parse_face_element(FaceElement *e)
{
    if (!is_token(TOKEN_INT))
        return false;

    e->v    = process_face_value(token.int_value, FACE_VALUE_V);
    e->type = FACE_V;
    next_token();

    if (match_token('/')) {
        if (match_token('/')) {
            e->type = FACE_VN;
            e->vn   = process_face_value(token.int_value, FACE_VALUE_N);
            expect_token(TOKEN_INT);
        } else {
            e->type = FACE_VT;
            e->vt   = process_face_value(token.int_value, FACE_VALUE_T);
            expect_token(TOKEN_INT);
            if (match_token('/')) {
                e->type = FACE_VTN;
                e->vn   = process_face_value(token.int_value, FACE_VALUE_N);
                expect_token(TOKEN_INT);
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

    *out1 = (Face){ .faces = { e11, e12, e13 }, .count = 3 };
    *out2 = (Face){ .faces = { e21, e22, e23 }, .count = 3 };
}

void parse_line(char *line);

typedef struct Mtl
{
    const char *name;
    float Ka; // ambient color
    float Kd; // diffuse color
    float Ks; // specular color
    float Ke; // emission color
    float d;  // opacity
    float Ns; //specular exponent

    const char *map_Kd;
    const char *map_bump;
} Mtl;

#define MAX_MATERIALS 256
typedef struct MtlLib
{
    Mtl materials[MAX_MATERIALS];
    int num_materials;
    int current_material;
} MtlLib;

static MtlLib mtllib;

void set_current_material(const char *name)
{
    forn(i, mtllib.num_materials)
    {
        Mtl m = mtllib.materials[i];

        if (m.name == name) {
            mtllib.current_material = i;
            return;
        }
    }

    mtllib.num_materials++;
    mtllib.current_material                        = mtllib.num_materials - 1;
    mtllib.materials[mtllib.current_material].name = name;
}

void set_material_map_kd(const char *name)
{
    mtllib.materials[mtllib.current_material].map_Kd = name;
}

int get_material_index(const char *name)
{
    forn(i, mtllib.num_materials)
    {
        Mtl m = mtllib.materials[i];

        if (m.name == name) {
            return i;
        }
    }

    return -1;
}

void parse_mtllib_line(char *line)
{
    stream = line;

    next_token();

    if (match_keyword(keywords[KEYWORD_NEWMTL])) {
        set_current_material(token.string_value);
        expect_token(TOKEN_NAME);
    } else if (match_keyword(keywords[KEYWORD_MAP_KD])) {
        set_material_map_kd(token.string_value);
        expect_token(TOKEN_NAME);
    }

    while (token.kind != TOKEN_EOL) {
        next_token();
    }
}

void parse_mtllib(const char *name)
{
    FILE *f;

    int nlines;
    char buf[256];
    char buf2[256];
    get_base_dir(MODEL, buf, 256);
    t_snprintf(buf2, 256, "%s%s", buf, name);
    char **lines = t_read_file_lines(buf2, &nlines);

    forn(i, nlines)
    {
        parse_mtllib_line(lines[i]);
    }

    // exit(0);
}

void parse_line(char *line)
{
    stream = line;

    next_token();

    if (match_keyword(keywords[KEYWORD_VERTEX])) {
        Vec3 v;

        v.x = read_number();
        v.y = read_number();
        v.z = read_number();

        t_arr_push(parser.vertices, v);

    } else if (match_keyword(keywords[KEYWORD_NORMAL])) {
        Vec3 v;

        v.x = read_number();
        v.y = read_number();
        v.z = read_number();

        t_arr_push(parser.normals, v);

    } else if (match_keyword(keywords[KEYWORD_TEXCOORD])) {
        Vec2 v;

        v.x = read_number();
        v.y = read_number();

        t_arr_push(parser.texcoords, v);

        match_token(TOKEN_FLOAT);
    } else if (match_keyword(keywords[KEYWORD_FACE])) {
        Face f = { 0 };

        FaceElement e;
        while (parse_face_element(&e)) {
            f.faces[f.count] = e;
            f.count++;
        }

        if (f.count == 4) {
            Face f1, f2;
            triangulate_face(&f, &f1, &f2);
            push_face(f1);
            push_face(f2);
        } else {
            push_face(f);
        }

    } else if (match_keyword(keywords[KEYWORD_GROUP])) {
        // t_printf("group %s\n", token.string_value);
        expect_token(TOKEN_NAME);
    } else if (match_keyword(keywords[KEYWORD_USE_MTL])) {
        parser_set_material(token.string_value);
        expect_token(TOKEN_NAME);
    } else if (match_keyword(keywords[KEYWORD_SMOOTH])) {
        if (token.kind == TOKEN_INT) {
            // t_printf("s %d\n", token.int_value);
            next_token();
        } else if (match_keyword(keywords[KEYWORD_OFF])) {
            // t_printf("s OFF\n");
        } else {
            // t_printf("Syntax error 's': %s\n", token.string_value);
            abort();
        }
    } else if (match_keyword(keywords[KEYWORD_MTLLIB])) {
        t_assert(token.kind == TOKEN_NAME);
        char *oldstream = stream;
        parse_mtllib(token.string_value);
        stream = oldstream;
        next_token();
    }

    expect_token(TOKEN_EOL);
}

typedef struct
{
    u32 vertex_offset;
    u32 vertex_byte_size;
    u32 normal_offset;
    u32 normal_byte_size;
    u32 texcoord_offset;
    u32 texcoord_byte_size;
    u32 material_id;
} PFAHeader;

void remove_indices()
{
    forn(idx, parser.num_objects)
    {
        Object *o       = parser.objects + idx;
        Face *obj_faces = o->faces;

        forn(i, t_arr_len(obj_faces))
        {
            Face f = obj_faces[i];

            t_assert(f.count == 3);

            forn(k, 3)
            {
                FaceElement e = f.faces[k];

                t_arr_push(o->output_vertices, parser.vertices[e.v - 1]);

                if (e.type == FACE_VN || e.type == FACE_VTN) {
                    t_arr_push(o->output_normals, parser.normals[e.vn - 1]);
                }

                if (e.type == FACE_VT || e.type == FACE_VTN) {
                    t_arr_push(o->output_texcoords, parser.texcoords[e.vt - 1]);
                }
            }
        }
    }
}

void build_normals()
{
    forn(idx, parser.num_objects)
    {
        Face *faces = parser.objects[idx].faces;

        for (int i = 1; i <= t_arr_len(parser.vertices); i++) {
            Vec3 sum = vec3(0, 0, 0);
            Vec3 v   = parser.vertices[i - 1];
            forn(k, t_arr_len(faces))
            {
                int ia = 0, ib = 0;
                Face *f = &faces[k];

                if (f->faces[0].v == i) {
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

                Vec3 a = parser.vertices[ia - 1];
                Vec3 b = parser.vertices[ib - 1];

                Vec3 e1 = vec3_sub(a, v);
                Vec3 e2 = vec3_sub(b, v);

                sum = vec3_add(sum, vec3_cross(e1, e2));
            }

            t_arr_push(parser.normals, vec3_normalized(sum));
        }

        forn(k, t_arr_len(faces))
        {
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
    }
}

u32 calculate_object_byte_size(Object *o)
{
    return 12 * t_arr_len(o->output_vertices) + 12 * t_arr_len(o->output_normals) + 8 * t_arr_len(o->output_texcoords) +
           sizeof(PFAHeader);
}

void output_binary()
{
    boolean built_normals = false;
    if (t_arr_len(parser.normals) == 0) {
        build_normals();
        built_normals = true;
    }

    remove_indices();

    FILE *bin = fopen("out.pfa", "wb");

    PFAHeader header   = { 0 };
    u32 current_offest = 0;

    fwrite(&parser.num_objects, sizeof(parser.num_objects), 1, bin);
    current_offest += sizeof(parser.num_objects);

    // We add one so we have 32bits of zeros before the objects' data.
    u32 projected_offset = current_offest + sizeof(u32) * (parser.num_objects + 1);
    current_offest       = projected_offset;
    forn(idx, parser.num_objects)
    {
        u32 os = calculate_object_byte_size(parser.objects + idx);
        fwrite(&projected_offset, sizeof(u32), 1, bin);
        projected_offset += os;
    }

    u32 z = 0;
    fwrite(&z, 4, 1, bin);

    forn(idx, parser.num_objects)
    {
        Vec3 *final_vertices  = parser.objects[idx].output_vertices;
        Vec3 *final_normals   = parser.objects[idx].output_normals;
        Vec2 *final_texcoords = parser.objects[idx].output_texcoords;

        u32 num_vertices  = t_arr_len(final_vertices);
        u32 num_normals   = t_arr_len(final_normals);
        u32 num_texcoords = t_arr_len(final_texcoords);
        u32 tempoff;

        u32 vertex_offset    = current_offest + sizeof(PFAHeader);
        u32 vertex_byte_size = num_vertices * 12;
        tempoff              = vertex_offset + vertex_byte_size;

        u32 normal_offset    = num_normals > 0 ? tempoff : 0;
        u32 normal_byte_size = num_normals * 12;
        tempoff += normal_byte_size;

        u32 texcoord_offset    = num_texcoords > 0 ? tempoff : 0;
        u32 texcoord_byte_size = 8 * num_texcoords;
        tempoff += texcoord_byte_size;

        fwrite(&vertex_offset, sizeof(u32), 1, bin);
        fwrite(&vertex_byte_size, sizeof(u32), 1, bin);
        fwrite(&normal_offset, sizeof(u32), 1, bin);
        fwrite(&normal_byte_size, sizeof(u32), 1, bin);
        fwrite(&texcoord_offset, sizeof(u32), 1, bin);
        fwrite(&texcoord_byte_size, sizeof(u32), 1, bin);

        u32 material_index = 0;
        if (parser.objects[idx].material_name) {
            material_index = get_material_index(parser.objects[idx].material_name);

            if (material_index < 0) {
                material_index = 0;
            } else {
                material_index++;
            }
        }

        t_printf("mi: %d at %d\n", material_index, ftell(bin));
        fwrite(&material_index, sizeof(u32), 1, bin);

        current_offest += sizeof(PFAHeader);

        if (t_arr_len(final_vertices) > 0) {
            header.vertex_offset    = current_offest;
            header.vertex_byte_size = t_arr_len(final_vertices) * 3 * 4;
            current_offest += header.vertex_byte_size;

            forn(i, t_arr_len(final_vertices))
            {
                fwrite((void *)final_vertices[i].values, sizeof(f32), 3, bin);
            }
        }

        if (t_arr_len(final_normals) > 0) {
            header.normal_offset    = current_offest;
            header.normal_byte_size = t_arr_len(final_normals) * 3 * 4;
            current_offest += header.normal_byte_size;

            forn(i, t_arr_len(final_normals))
            {
                fwrite((void *)final_normals[i].values, sizeof(f32), 3, bin);
            }
        }

        if (t_arr_len(final_texcoords) > 0) {
            header.texcoord_offset    = current_offest;
            header.texcoord_byte_size = t_arr_len(final_texcoords) * 2 * 4;
            current_offest += header.texcoord_byte_size;

            forn(i, t_arr_len(final_texcoords))
            {
                fwrite((void *)final_texcoords[i].values, sizeof(f32), 2, bin);
            }
        }

        t_printf("%d vertices, %d tex coords.\n", t_arr_len(final_vertices), t_arr_len(final_texcoords));
    }

    t_printf("%d bytes written.\n", current_offest);

    fclose(bin);
}

void output_material_library()
{
    FILE *b = fopen("out.pml", "wb");

    t_assert(b);

    u32 current_offset;
    u32 v = mtllib.num_materials;
    fwrite(&v, sizeof(u32), 1, b);

    u32 offsets[MAX_MATERIALS];

    fseek(b, sizeof(u32) * mtllib.num_materials, SEEK_CUR);

    forn(i, mtllib.num_materials)
    {
        offsets[i] = ftell(b);
        Mtl m      = mtllib.materials[i];

        v = strlen(m.name) + 1;
        fwrite(&v, sizeof(u32), 1, b);
        fwrite(m.name, sizeof(char), v, b);

        if (m.map_Kd) {
            v = strlen(m.map_Kd) + 1;
            fwrite(&v, sizeof(u32), 1, b);
            fwrite(m.map_Kd, sizeof(char), v, b);
        } else {
            v = 0;
            fwrite(&v, sizeof(u32), 1, b);
        }
    }

    fseek(b, sizeof(u32), SEEK_SET);
    forn(i, mtllib.num_materials)
    {
        v = offsets[i];
        fwrite(&v, sizeof(u32), 1, b);
    }

    fclose(b);
}

int main(int argc, char **argv)
{
    t_printf("obj test \n");

    int nlines;
    char **lines;

    lines = t_read_file_lines(MODEL, &nlines);

    keywords[KEYWORD_VERTEX]   = string_intern("v");
    keywords[KEYWORD_NORMAL]   = string_intern("vn");
    keywords[KEYWORD_TEXCOORD] = string_intern("vt");
    keywords[KEYWORD_FACE]     = string_intern("f");
    keywords[KEYWORD_GROUP]    = string_intern("g");
    keywords[KEYWORD_USE_MTL]  = string_intern("usemtl");
    keywords[KEYWORD_MTLLIB]   = string_intern("mtllib");
    keywords[KEYWORD_SMOOTH]   = string_intern("s");
    keywords[KEYWORD_OFF]      = string_intern("off");
    keywords[KEYWORD_OBJECT]   = string_intern("o");
    keywords[KEYWORD_NEWMTL]   = string_intern("newmtl");
    keywords[KEYWORD_MAP_KD]   = string_intern("map_Kd");

    parser_init();

    forn(i, nlines)
    {
        parser.current_line = i;
        parse_line(lines[i]);
    }

    output_binary();
    output_material_library();

    return 0;
}
