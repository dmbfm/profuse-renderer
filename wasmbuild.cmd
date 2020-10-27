clang --target=wasm32 -nostdlib -fvisibility=hidden -O1 -c src\main.c
wasm-ld --export-dynamic -no-entry -allow-undefined --import-memory --export=__heap_base  -z stack-size=1024 main.o  -o main.wasm
rem wabt-1.0.19\bin\wasm2wat add.wasm
