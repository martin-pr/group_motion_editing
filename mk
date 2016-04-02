#!/bin/bash

if [ ! -e bin ]; then
    mkdir bin
fi

cd bin
cmake .. || exit 1
make $@ || exit 1
cd ..
