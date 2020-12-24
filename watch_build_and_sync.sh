#!/bin/sh
find src | entr -s "./synctest.sh & python build.py wasm"
