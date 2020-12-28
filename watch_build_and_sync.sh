#!/bin/sh
find src js | entr -s "./synctest.sh & python build.py wasm"

