let memory = new WebAssembly.Memory({ initial: 512 });
let table = new WebAssembly.Table({
    element: "anyfunc",
    initial: 100,
    maximum: 1000
});
let i32, u32, u8, f64;
let canvas, ctx;
let mouseX, mouseY;
let mouseXptr, mouseYptr;
let instance;
let malloc;

function initMemoryViews() {
    u32 = new Uint32Array(memory.buffer);
    u8 = new Uint8Array(memory.buffer);
    f64 = new Float64Array(memory.buffer);
    i32 = new Int32Array(memory.buffer);
}

class ObjectPool {
    constructor() {
        this.pool = [];
    }

    add(o) {
        this.pool.push(o);
        return this.pool.length;
    }

    get(id) {
        return this.pool[id - 1];
    }
}

let glPrograms = new ObjectPool();
let glShaders = new ObjectPool();

function addShader(shader) {
    glShaders.push(shader);
    return glShaders.length;
}

function getShader(id) {
    return glShaders[id];
}

function addProgram(program) {
    glPrograms.push(program);
    return glPrograms.length;
}

function getProgram(id) {
    return glPrograms[id];
}

function initCanvasEventListeners() {
    if (!canvas) {
        console.warn("[initCanvasEventListeners]: Canvas is null!");
        return;
    }

    canvas.addEventListener("mousemove", e => {
        var rect = canvas.getBoundingClientRect();
        u32[mouseXptr / 4] = e.clientX - rect.left;
        u32[mouseYptr / 4] = e.clientY - rect.top;
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

const wasm_exports = {
    wasm_get_memory_size,
    wasm_print_line,
    wasm_print_i32,
    wasm_memory_grow
};
