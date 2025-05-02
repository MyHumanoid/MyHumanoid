#!/bin/bash
rm -r build-web
EM_CONFIG=$HOME/.emscripten
emcmake cmake \
-S . \
-B build-web \
-D MH_USE_GLES=ON \
-D MH_USE_SYSTEM_FMT=OFF \
-D MH_USE_SYSTEM_SDL=OFF \
-D MH_USE_SYSTEM_GLM=OFF \
-D MH_USE_SYSTEM_PHYSFS=OFF
