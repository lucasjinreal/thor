#!/usr/bin/env bash
mkdir build
cd build
cmake ..
make -j2
sudo make install

echo "thor installed."