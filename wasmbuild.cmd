SET INCLUDE_DIRS=-Isrc\toolbox -Isrc\coffee
clang --target=wasm32 %INCLUDE_DIRS% -nostdlib -fvisibility=hidden -O3 -c src\unity_build.c -o build\main_wasm.o
wasm-ld --export-dynamic -no-entry -allow-undefined --import-memory --export=__heap_base  -z stack-size=2097152 build\main_wasm.o  -o build\main.wasm
python genwasmjs.py
rem wabt-1.0.19\bin\wasm2wat add.wasm
