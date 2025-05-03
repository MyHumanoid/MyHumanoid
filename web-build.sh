#!/bin/bash
rm -r build-web
EM_CONFIG=$HOME/.emscripten
emcmake cmake -S . -B build-web
