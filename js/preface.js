let memory = new WebAssembly.Memory({ initial: 1000 });
let i32, u32, u8, f64;
let canvas, ctx;
let mouseX, mouseY;
let mouseXptr, mouseYptr;
let instance;

let glPrograms = [];
let glShaders = [];

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
        u32[mouseXptr/4] = e.clientX - rect.left;
        u32[mouseYptr/4] = e.clientY - rect.top;
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
