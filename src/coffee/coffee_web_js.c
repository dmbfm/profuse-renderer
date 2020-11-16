#include "coffee.h"
#include "coffee_web.h"
#include "coffee_gl.h"

// clang-format off

extern float c__wasm_js_sqrtf(float x);
WASM_JS(
        function c__wasm_js_sqrtf(x) {
            return Math.sqrt(x);
        }
)

extern float c__wasm_js_tanf(float x);
WASM_JS(
        function c__wasm_js_tanf(x) {
            return Math.tan(x);
        }
)

extern float c__wasm_js_sinf(float x);
WASM_JS(
        function c__wasm_js_sinf(x) {
            return Math.sin(x);
        }
)

extern float c__wasm_js_cosf(float x);
WASM_JS(
        function c__wasm_js_cosf(x) {
            return Math.cos(x);
        }
)

extern u32 c__wasm_js_get_memory_size(void);
WASM_JS(
    function c__wasm_js_get_memory_size() {
        return memory.buffer.byteLength;
    }
)


extern void c__wasm_js_init_canvas(int width, int height);
WASM_JS(
        function c__wasm_js_init_canvas(width, height) {
            canvas = document.createElement("canvas");
            canvas.width = width;
            canvas.height = height;
            document.body.appendChild(canvas);
            initCanvasEventListeners();
        }
)

extern void c__wasm_js_canvas_set_curstor_style(int style);
WASM_JS(
        function c__wasm_js_canvas_set_curstor_style(style) {
        switch(style) {
            case 0:
                canvas.style.cursor = "default";
                break;
            case 1:
                canvas.style.cursor = "none";
            default: break;
        }
        }
       )

extern void c__wasm_js_start_main_loop(Coffee *p);
WASM_JS(
        function c__wasm_js_start_main_loop(p) {
        let f = () => {
            instance.exports.frame(p);
            requestAnimationFrame(f);
        }
        f();
}
)

// NOTE: mouse{X,Y}ptr are defined in 'preface.js', and are updated there via event listeners.
// We just need to set them here at startup.
extern void c__wasm_js_set_mouse_pointers(i32 *x, i32 *y);
WASM_JS(
        function c__wasm_js_set_mouse_pointers(x,y) {
            mouseXptr = x;
            mouseYptr = y;
        }
)


extern void c__wasm_js_set_mouse_down_pointer(u8 *ptr);
WASM_JS(
        function c__wasm_js_set_mouse_down_pointer(ptr) {
            mouseDownPtr = ptr;
            u8[mouseDownPtr] = 0;

            canvas.addEventListener("mousedown", e => {
                u8[mouseDownPtr] =  1;
            });

        canvas.addEventListener("mouseup", e => {
                u8[mouseDownPtr] = 0;
            });

        }
)

extern void c__wasm_js_set_mouse_wheel_pointer(i32 *ptr);
WASM_JS(
        function c__wasm_js_set_mouse_wheel_pointer(ptr) {
             let mouseWheelPtr = ptr;
            u32[mouseWheelPtr / 4] = 0;

            canvas.addEventListener("wheel", e => {
                e.preventDefault();
                u32[mouseWheelPtr / 4] = e.deltaY;
            })
        }
)

extern void c__wasm_js_now(double *out);
WASM_JS(
        function c__wasm_js_now(ptr) {
            f64[ptr/8] = performance.now();
        }
)

extern void c__wasm_js_init_gl_context();
WASM_JS(
        function c__wasm_js_init_gl_context() {
            if (!canvas) { throw("No canvas!"); }

            ctx = canvas.getContext("webgl");

            if (!ctx) { throw("Failed to create context!"); }
        }
)

extern void c__wasm_js_gl_clear_color(float r, float g, float b, float a);
WASM_JS(
        function c__wasm_js_gl_clear_color(r,g,b,a) {
            ctx.clearColor(r,g,b,a);
        }
)

extern void c__wasm_js_gl_clear(GLbitfield mask);
WASM_JS(
        function c__wasm_js_gl_clear(mask) {
            ctx.clear(mask);
        }
)

extern GLuint c__wasm_js_glCreateShader(GLenum shaderType); 
WASM_JS(
        function c__wasm_js_glCreateShader(shaderType) {
            return glShaders.add(ctx.createShader(shaderType));
        }
)

extern GLuint c__wasm_js_glCreateProgram(void); 
WASM_JS(
        function c__wasm_js_glCreateProgram() {
            return glPrograms.add(ctx.createProgram());
        }
)


extern void c__wasm_js_glCompileShader(GLuint shader);
WASM_JS(
        function c__wasm_js_glCompileShader(shader) {
            ctx.compileShader(glShaders.get(shader));
        }
)

extern void c__wasm__js_glDeleteShader(GLuint shader);
WASM_JS(
        function c__wasm__js_glDeleteShader(shader) {
            ctx.deleteShader(glShaders.get(shader));
        }
)

extern GLuint c__wasm__js_getShaderParameter(GLuint shader, GLenum pname);
WASM_JS(
        function c__wasm__js_getShaderParameter(shader, pname) {
            return ctx.getShaderParameter(glShaders.get(shader), pname);
        }
)

void c__wasm__js_glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    *params = c__wasm__js_getShaderParameter(shader, pname);
}

extern void c__wasm__js_glShaderSource(GLuint shader, char *src);
WASM_JS(
        function c__wasm__js_glShaderSource(shader, ptr) {
            let string = decodeStringAt(ptr);
            ctx.shaderSource(glShaders.get(shader), string);
        }
)

void c__wasm__glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
   //TODO: For now we only accept a single string. Fix this to conform to standard later.
   c__wasm__js_glShaderSource(shader, (char *) string[0]);
}

extern void c__wasm__js_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
WASM_JS(
        function c__wasm__js_glGetShaderInfoLog(shader, bufSize, lenPtr, strPtr) {
            let log = ctx.getShaderInfoLog(glShaders.get(shader));
            if (strPtr) {
            encodeStringAt(log, strPtr, bufSize);
                if (lenPtr) {
                    u32[lenPtr / 4] = log.length;
                }
            }
        }
)

extern void c__wasm__js_glLinkProgram(GLuint program);
WASM_JS(
        function c__wasm__js_glLinkProgram(program) {
           ctx.linkProgram(glPrograms.get(program)); 
        }
)

extern GLuint c__wasm__js_glCrateProgram(void);
WASM_JS(
        function c__wasm__js_glCrateProgram() {
            return addProgram(ctx.createProgram());
        }
)

extern void c__wasm__js_glAttachShader(GLuint program, GLuint shader);
WASM_JS(
        function c__wasm__js_glAttachShader(program, shader) {
            ctx.attachShader(glPrograms.get(program), glShaders.get(shader));
        }
)

extern void c__wasm__js_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
WASM_JS(
        function c__wasm__js_glGetProgramiv(program, pname, ptr) {
            let value = ctx.getProgramParameter(glPrograms.get(program), pname);

            u32[ptr/4] = value;
       }
)

extern void c__wasm__js_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
WASM_JS(
        function c__wasm__js_glGetProgramInfoLog(program, bufSize, lenPtr, strPtr) {
            let log = ctx.getProgramInfoLog(glPrograms.get(program));
            if (strPtr) {
            encodeStringAt(log, strPtr, bufSize);
                if (lenPtr) {
                    u32[lenPtr / 4] = log.length;
                }
            }
        }
)

extern void c__wasm__js_glUseProgram(GLuint program);
WASM_JS(
        function c__wasm__js_glUseProgram(program) {
            ctx.useProgram(glPrograms.get(program));
        }
)


extern void c__wasm__js_glDrawArrays(GLenum mode, GLint first, GLsizei count);
WASM_JS(
        function c__wasm__js_glDrawArrays(mode, first, count) {
            ctx.drawArrays(mode, first, count);
        }
)

WASM_JS(
        let glBuffers = new ObjectPool();
)

extern void c__wasm__js_glGenBuffers(GLsizei n, GLuint *buffers);
WASM_JS(
        function c__wasm__js_glGenBuffers(n, buffersPtr) {
            for (let i = 0; i < n; i++) {
                u32[buffersPtr/4 + i] = glBuffers.add(ctx.createBuffer());
            }
        }
)

extern void c__wasm__js_glBindBuffer(GLenum target, GLuint buffer);
WASM_JS(
        function c__wasm__js_glBindBuffer(target, buffer) {
            ctx.bindBuffer(target, glBuffers.get(buffer));
        }
)

extern void c__wasm__js_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
WASM_JS(
        function c__wasm__js_glBufferData(target, size, dataPtr, usage) {
            buffer = memory.buffer.slice(dataPtr, dataPtr + size);
            ctx.bufferData(target, buffer, usage);
        }
)

extern void c__wasm__js_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
WASM_JS(
        function c__wasm__js_glBufferSubData(target, offset, size, dataPtr) {
            buffer = memory.buffer.slice(dataPtr, dataPtr + size);
            ctx.bufferSubData(target, offset, buffer);
        }
)

extern void c__wasm__js_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
WASM_JS(
        function c__wasm__js_glVertexAttribPointer(index, size, type, normalized, stride, pointer) {
            ctx.vertexAttribPointer(index, size, type, normalized, stride, pointer);
        }
)

extern void c__wasm__js_glEnableVertexAttribArray(GLuint index);
WASM_JS(
        function c__wasm__js_glEnableVertexAttribArray(index) {
            ctx.enableVertexAttribArray(index);
        }
)

extern void c__wasm__js_glDisableVertexAttribArray(GLuint index);
WASM_JS(
        function c__wasm__js_glDisableVertexAttribArray(index) {
            ctx.disableVertexAttribArray(index);
        }
)

extern void c__wasm__js_glGetIntegerv(GLenum value, int *data);
WASM_JS(
        function c__wasm__js_glGetIntegerv(value, data) {
            return ctx.getParameter(value);
        }
)

const GLubyte* c__wasm__glGetString(GLenum name)
{
    return (const unsigned char *) "glGetString not implemented!";
}

extern void c__wasm__js_glVertexAttrib1f(GLuint index, float value);
WASM_JS(
        function c__wasm__js_glVertexAttrib1f(index, value) {
            ctx.vertexAttrib1f(index, value);
        }
)

WASM_JS(
    let uniformLocationsPool = new ObjectPool();
)

extern GLint c__wasm_js_glGetUniformLocation(GLuint program, const GLchar *pname);
WASM_JS(
        function c__wasm_js_glGetUniformLocation(program, strPtr) {
            let name = decodeStringAt(strPtr);

            let loc = ctx.getUniformLocation(glPrograms.get(program), name);

            if (loc == null) {
                return -1;
            }

            return uniformLocationsPool.add(loc);
        }
)

// TODO: For now we assume count=1. Fix this later. See: https://www.gamedev.net/forums/topic/658191-webgl-how-to-send-an-array-of-matrices-to-the-vertex-shader/5163499/
extern void c__wasm_js_glUniformMatrix4fv(GLuint location, GLsizei count, GLboolean transpose, const GLfloat *value);
WASM_JS(
        function c__wasm_js_glUniformMatrix4fv(location, count, transpose, ptr) {
            let mat = new Float32Array(memory.buffer, ptr, 16);

            //console.log(mat);

            ctx.uniformMatrix4fv(uniformLocationsPool.get(location), transpose, mat);
        }
)

extern void c__wasm_js_glUniform3fv(GLuint location, const GLfloat *value);
WASM_JS(
        function c__wasm_js_glUniform3fv(loc, ptr) {
            let arr = new Float32Array(memory.buffer, ptr, 3);

            ctx.uniform3fv(uniformLocationsPool.get(loc), arr);
        }
)

extern void c__wasm_js_glEnable(GLenum cap);
WASM_JS(
        function c__wasm_js_glEnable(cap) {
            ctx.enable(cap);
        }
)


// clang-format on
