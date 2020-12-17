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

    //mouseXptr = instance.exports.malloc(4);
    //mouseYptr = instance.exports.malloc(4);

    //console.log(instance.exports.malloc(100));
    //console.log(instance.exports.malloc(100));
});
