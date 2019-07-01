#!/bin/bash
# this script build cao library automatically

sudo chmod -R 777 *
if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform
    printf "======== installing dependencies on macOS ============"
    brew install glog

    mkdir build
    cd build
    cmake ..
    make all -j8
    sudo make install
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    printf "======== installing dependencies on linux ============"
    sudo apt install libglog-dev
    
    mkdir build
    cd build
    cmake ..
    make all -j8
    sudo make install

elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under 32 bits Windows NT platform
    echo "this is windows 32bit, cao not support yet."
    
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Do something under 64 bits Windows NT platform
    echo "this is windows 64bit, cao not support yet."
fi