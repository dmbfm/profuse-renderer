let env = {
  memory,
  ...funcs,
};

WebAssembly.instantiateStreaming(fetch("build/main.wasm"), { env }).then(
  (result) => {
    instance = result.instance;
    u32 = new Uint32Array(memory.buffer);
    u8 = new Uint8Array(memory.buffer);
    f64 = new Float64Array(memory.buffer);
    i32 = new Int32Array(memory.buffer);

    instance.exports.main(0, 0);

    //mouseXptr = instance.exports.malloc(4);
    //mouseYptr = instance.exports.malloc(4);

    //console.log(instance.exports.malloc(100));
    //console.log(instance.exports.malloc(100));

  }
);
