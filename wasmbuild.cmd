SET INCLUDE_DIRS=-Isrc\toolbox -Isrc\coffee
SET DEBUG_FLAGS= -DPROFUSE_DEBUG
clang --target=wasm32 %INCLUDE_DIRS% %DEBUG_FLAGS% -nostdlib -fvisibility=hidden -O3 -c src\unity_build.c -o build\main_wasm.o
wasm-ld --export-dynamic --import-table -no-entry -allow-undefined --import-memory --export=__heap_base  -z stack-size=10097152 build\main_wasm.o  -o build\main.wasm
python genwasmjs.py
rem wabt-1.0.19\bin\wasm2wat add.wasm
