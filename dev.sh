#!/bin/sh
#find src js | entr -s "python build.py wasm"

(trap 'kill 0' SIGINT; find src js | entr -s "python build.py wasm" & wasm-server & ./synctest.sh)
