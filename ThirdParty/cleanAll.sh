#!/bin/bash

cd SDL2-2.0.8
rm -rf build
rm -rf sdl2-install
cd ..

cd SDL2_ttf-2.0.14/
make distclean
rm -rf sdl2-ttf-install
cd ..

cd freetype-2.4.0
make distclean
rm -rf freetype-install
cd ..

cd assimp
rm -rf build
cd ..

cd glew-2.1.0
make distclean
cd ..

cd libjpeg-turbo
rm -rf build
cd ..

cd libpng-1.6.34
rm -rf build
cd ..

cd openal-soft
rm -rf build
cd ..

