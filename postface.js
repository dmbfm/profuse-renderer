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
