let env = {
    memory,
    __indirect_function_table: table,
    ...funcs,
    ...wasm_exports
};

WebAssembly.instantiateStreaming(fetch("build/main.wasm"), { env }).then(result => {
    instance = result.instance;

    initMemoryViews();

    instance.exports.main(0, 0);
    malloc = instance.exports.malloc;

    const frame = dt => {
        if (instance.exports.frame(dt)) requestAnimationFrame(frame);
        else instance.exports.shutdown();
    };

    frame(0);
});
