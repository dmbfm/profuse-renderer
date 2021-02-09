let memory = new WebAssembly.Memory({ initial: 512 });
let table = new WebAssembly.Table({
    element: "anyfunc",
    initial: 100,
    maximum: 1000
});
let i32, u32, u8, f64;
let canvas, gl;
let mouseX, mouseY;
let instance;
let malloc;

function initMemoryViews() {
    u32 = new Uint32Array(memory.buffer);
    u8 = new Uint8Array(memory.buffer);
    f64 = new Float64Array(memory.buffer);
    i32 = new Int32Array(memory.buffer);
}

function wasm_init_canvas(width, height) {
    canvas = document.createElement("canvas");
    canvas.width = width;
    canvas.height = height;
    document.body.appendChild(canvas);
    initCanvasEventListeners();
}

function wasm_init_gl_context() {
    if (!canvas) {
        throw "No canvas!";
    }

    gl = canvas.getContext("webgl", { alpha: false });

    if (!gl) {
        throw "Failed to create context!";
    }
}

function initCanvasEventListeners() {
    if (!canvas) {
        console.warn("[initCanvasEventListeners]: Canvas is null!");
        return;
    }

    canvas.addEventListener("mousemove", e => {
        var rect = canvas.getBoundingClientRect();
        mouseX = e.clientX - rect.left;
        mouseY = e.clientY - rect.top;
    });
}

function decodeStringAt(pointer) {
    let p = pointer;
    let cc;
    let str = "";

    while ((cc = u8[p++])) {
        str += String.fromCharCode(cc);
    }
    return str;
}

function decodeLenStringAt(pointer, len) {
    let p = pointer;
    let cc;
    let str = "";

    while (len) {
        cc = u8[p++];
        str += String.fromCharCode(cc);
        len--;
    }
    return str;
}

function encodeStringAt(str, ptr, len) {
    for (let i = 0; i < str.length && i < len; i++) {
        u8[ptr++] = str.charCodeAt(i);
    }

    u8[ptr] = 0;
}

//  Imported library

function wasm_get_memory_size() {
    return memory.buffer.byteLength;
}

function wasm_print_line(ptr) {
    console.log(decodeStringAt(ptr));
}

function wasm_print_i32(value) {
    console.log(value);
}

function wasm_mouse_position(xptr, yptr) {
    i32[xptr / 4] = mouseX;
    i32[yptr / 4] = mouseY;
}

function wasm_memory_grow(pages) {
    let result;
    try {
        result = memory.grow(pages);
    } catch {
        console.warn("[wasm_memory_grow]: Failed to grow memory!");
    }

    initMemoryViews();

    return result;
}

let wasm_exports = {
    wasm_get_memory_size,
    wasm_print_line,
    wasm_print_i32,
    wasm_memory_grow,
    wasm_init_canvas,
    wasm_init_gl_context,
    wasm_mouse_position
};
