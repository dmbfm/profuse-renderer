let memory = new WebAssembly.Memory({ initial: 1000 });
let u32, u8, f64;
let canvas, ctx;
let mouseX, mouseY;
let mouseXptr, mouseYptr;
let instance;

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
