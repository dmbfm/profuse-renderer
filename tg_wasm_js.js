let memory = new WebAssembly.Memory({ initial: 100 });
let u32, u8;

function decodeStringAt(pointer) {
  let p = pointer;
  let cc;
  let str = "";

  while ((cc = u8[p++])) {
    str += String.fromCharCode(cc);
  }
  return str;
}

function tg_wasm_js_sqrtf(x) { return Math.sqrt(x); }
function tg__wasm_js_get_memory_size() {
    return memory.buffer.byteLength;
}
function tg__wasm_js_print_line(p) { 
  console.log(decodeStringAt(p)); 
}


let funcs = {

    tg_wasm_js_sqrtf,

    tg__wasm_js_get_memory_size,

    tg__wasm_js_print_line,

}

let env = {
  memory,
  ...funcs,
};

let instance;
WebAssembly.instantiateStreaming(fetch("./main.wasm"), { env }).then(
  (result) => {
        instance = result.instance;
        u32 = new Uint32Array(memory.buffer);
        u8 = new Uint8Array(memory.buffer);

        instance.exports.main();

        console.log(instance.exports.malloc(100));
        console.log(instance.exports.malloc(100));
  }
);

