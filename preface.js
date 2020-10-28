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
