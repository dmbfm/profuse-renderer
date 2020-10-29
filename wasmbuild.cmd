clang --target=wasm32 -nostdlib -fvisibility=hidden -O2 -c src\main.c -o build\main_wasm.o
wasm-ld --export-dynamic -no-entry -allow-undefined --import-memory --export=__heap_base  -z stack-size=2097152 build\main_wasm.o  -o build\main.wasm
python genwasmjs.py
rem wabt-1.0.19\bin\wasm2wat add.wasm
