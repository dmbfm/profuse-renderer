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
let glBuffers = new ObjectPool();
let glUniformLocations = new ObjectPool();
let glTextures = new ObjectPool();

const GL_INFO_LOG_LENGTH = 35716;

function glUseProgram(program) {
    gl.useProgram(glPrograms.get(program));
}

function glDrawArrays(mode, first, count) {
    gl.drawArrays(mode, first, count);
}

function glDrawElements(mode, count, type, offset) {
    gl.drawElements(mode, count, type, offset);
}

function glClearColor(r, g, b, a) {
    gl.clearColor(r, g, b, a);
}

function glClear(mask) {
    gl.clear(mask);
}

function glCreateShader(type) {
    let s = gl.createShader(type);

    return glShaders.add(s);
}

function glCompileShader(shader) {
    gl.compileShader(glShaders.get(shader));
}

function glDeleteShader(shader) {
    gl.deleteShader(glShaders.get(shader));
}

function infologWrite(log, bufSize, lengthPtr, infoLogPtr) {
    if (infoLogPtr) {
        encodeStringAt(log, infoLogPtr, bufSize);

        if (lengthPtr) {
            u32[lengthPtr / 4] = log.length;
        }
    }
}

function glGetShaderInfoLog(shader, bufSize, lengthPtr, infoLogPtr) {
    let log = gl.getShaderInfoLog(glShaders.get(shader));

    infologWrite(log, bufSize, lengthPtr, infoLogPtr);
}

function glGetShaderiv(shader, pname, paramsPtr) {
    if (pname == GL_INFO_LOG_LENGTH) {
        let log = gl.getShaderInfoLog(glShaders.get(shader));
        i32[paramsPtr / 4] = log.length + 1;
        return;
    }

    let p = gl.getShaderParameter(glShaders.get(shader), pname);
    i32[paramsPtr / 4] = p;
}

function glCreateProgram() {
    let p = gl.createProgram();
    return glPrograms.add(p);
}

function glShaderSource(shader, count, ptrToStringArray, lengthPtr) {
    let s = "";
    for (let i = 0; i < count; i++) {
        let sPtr = i32[ptrToStringArray / 4 + i];
        if (lengthPtr) {
            let len = i32[lengthPtr / 4 + i];
            s += decodeLenStringAt(sPre, len);
        } else {
            s += decodeStringAt(sPtr);
        }
    }

    gl.shaderSource(glShaders.get(shader), s);
}

function glLinkProgram(program) {
    gl.linkProgram(glPrograms.get(program));
}

function glDeleteProgram(program) {
    gl.deleteProgram(glPrograms.get(program));
}

function glAttachShader(program, shader) {
    gl.attachShader(glPrograms.get(program), glShaders.get(shader));
}

function glGetProgramiv(program, pname, paramsPtr) {
    if (pname == GL_INFO_LOG_LENGTH) {
        let log = gl.getProgramInfoLog(glPrograms.get(program));
        i32[paramsPtr / 4] = log.length + 1;
        return;
    }

    let p = gl.getProgramParameter(glPrograms.get(program), pname);
    i32[paramsPtr / 4] = p;
}

function glGetProgramInfoLog(program, bufSize, lengthPtr, infoLogPtr) {
    let log = gl.getProgramInfoLog(glPrograms.get(program));

    infologWrite(log, bufSize, lengthPtr, infoLogPtr);
}

function drawArrays(mode, first, count) {
    gl.drawArrays(mode, first, count);
}

function drawElements(mode, count, type, offset) {
    gl.drawElements(mode, count, type, offset);
}

function glGenVertexArrays(n, arraysPtr) {
    console.warn("Not implemented", n, arraysPtr);

    return 0;
}

function glBindVertexArray(vertexArray) {
    console.warn("Not implemented", vertexArray);
}

function glVertexAttrib1f(index, v0) {
    gl.vertexAttrib1f(index, v0);
}

function glGetString(name) {
    console.warn("Not implemented", name);

    return 0;
}

function glGetIntegerv(pname, paramsPtr) {
    let p = gl.getParameter(pname);

    if (typeof p === "number") {
        i32[paramsPtr / 4] = p;
    }
}

function glGenBuffers(n, buffersPtr) {
    for (let i = 0; i < n; i++) {
        let b = gl.createBuffer();
        u32[buffersPtr / 4 + i] = glBuffers.add(b);
    }
}

function glBindBuffer(target, buffer) {
    gl.bindBuffer(target, glBuffers.get(buffer));
}

function glBufferData(target, size, dataPtr, usage) {
    if (dataPtr) {
        view = new DataView(memory.buffer, dataPtr, size);
        gl.bufferData(target, view, usage);
    } else {
        gl.bufferData(target, size, usage);
    }
}

function glBufferSubData(target, offset, size, dataPtr) {
    view = new DataView(memory.buffer, dataPtr, size);
    gl.bufferSubData(target, offset, view, usage);
}

function glVertexAttribPointer(index, size, type, normalized, stride, pointer) {
    gl.vertexAttribPointer(index, size, type, normalized, stride, pointer);
}

function glEnableVertexAttribArray(index) {
    gl.enableVertexAttribArray(index);
}

function glDisableVertexAttribArray(index) {
    gl.disableVertexAttribArray(index);
}

function glEnable(name) {
    gl.enable(name);
}

function glGetUniformLocation(program, namePtr) {
    let string = decodeStringAt(namePtr);
    return glUniformLocations.add(gl.getUniformLocation(program, string));
}

function glUniformMatrix4fv(location, count, transpose, floatArrayPtr) {
    for (let i = 0; i < count; i++) {
        array = new Float32Array(memory.buffer, floatArrayPtr + 16 * 4 * i, 16);
        gl.uniformMatrix4fv(glUniformLocations.get(location), transpose, array);
    }
}

function glUniform3fv(location, ptr) {
    let i = ptr / 4;
    gl.uniform3f(glUniformLocations.get(location), f32[i], f32[i + 1], f32[i + 2]);
}

function glUniform1i(location, value) {
    gl.uniform1i(glUniformLocations.get(location), value);
}

function glGenTextures(n, ptr) {
    for (let i = 0; i < n; i++) {
        u32[ptr / 4 + i] = glTextures.add(gl.createTexture());
    }
}

function glActiveTexture(n) {
    gl.activeTexture(n);
}

function glBindTexture(target, texture) {
    gl.bindTexture(target, glTextures.get(texture));
}

function glTexImage2D(
    target,
    level,
    internalformat,
    width,
    height,
    border,
    format,
    type,
    pixelsPtr
) {
    view = new DataView(memory.buffer, pixelsPtr);
    gl.texImage2D(target, level, internalformat, width, height, border, format, type, view);
}

function glTexParameteri(target, pname, param) {
    gl.texParameteri(target, pname, param);
}

function glPixelStorei(pname, param) {
    gl.pixelStorei(pname, param);
}

function glBlendFunc(sfactor, dfactor) {
    gl.blendFunc(sfactor, dfactor);
}

function glGenerateMipmap(target) {
    gl.generateMipmap(target);
}

function glViewport(x, y, width, height) {
    gl.viewport(x, y, width, height);
}

wasm_exports = {
    ...wasm_exports,
    glClearColor,
    glClear,
    glCreateShader,
    glCompileShader,
    glDeleteShader,
    glGetShaderInfoLog,
    glGetShaderiv,
    glCreateProgram,
    glShaderSource,
    glLinkProgram,
    glDeleteProgram,
    glAttachShader,
    glGetProgramiv,
    glGetProgramInfoLog,
    glUseProgram,
    glDrawArrays,
    glDrawElements,
    glGenVertexArrays,
    glBindVertexArray,
    glGetIntegerv,
    glGenBuffers,
    glBindBuffer,
    glBufferData,
    glBufferSubData,
    glVertexAttribPointer,
    glEnableVertexAttribArray,
    glDisableVertexAttribArray,
    glEnable,
    glGetUniformLocation,
    glGenTextures,
    glActiveTexture,
    glBindTexture,
    glTexParameteri,
    glPixelStorei,
    glBlendFunc,
    glGenerateMipmap,
    glViewport
};
