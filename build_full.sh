#!/bin/bash
##
## Copyright (c) 2020 Fagang Jin.
##
## This file is part of thor
## (see manaai.cn).
##
## Licensed to the Apache Software Foundation (ASF) under one
## or more contributor license agreements.  See the NOTICE file
## distributed with this work for additional information
## regarding copyright ownership.  The ASF licenses this file
## to you under the Apache License, Version 2.0 (the
## "License"); you may not use this file except in compliance
## with the License.  You may obtain a copy of the License at
##
##   http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing,
## software distributed under the License is distributed on an
## "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
## KIND, either express or implied.  See the License for the
## specific language governing permissions and limitations
## under the License.
##
# this script build cao library automatically

#sudo apt install -y libopencv-dev
sudo apt install -y libfreetype6-dev
sudo apt install -y libcurl4-openssl-dev
sudo apt install -y libeigen3-dev
sudo apt-get install -y libprotobuf-dev
sudo apt install -y protobuf-compiler


rm -r build/ > /dev/null

sudo chmod -R 777 *
if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform
    printf "======== installing dependencies on macOS ============"
    brew install glog

    mkdir build
    cd build
    cmake -DUSE_OPENCV=ON -DUSE_EIGEN=ON -DUSE_PROTOBUF=ON -DUSE_CV_EX=ON -DUSE_CURL=ON ..
    make all -j8
    sudo make install
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    printf "======== installing dependencies on linux ============"
    sudo apt install -y libglog-dev
    
    mkdir build
    cd build
    cmake -DUSE_OPENCV=ON -DUSE_EIGEN=ON -DUSE_PROTOBUF=ON -DUSE_CV_EX=ON -DUSE_CURL=ON ..
    make all -j8
    sudo make install

elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under 32 bits Windows NT platform
    echo "this is windows 32bit, cao not support yet."
    
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Do something under 64 bits Windows NT platform
    echo "this is windows 64bit, cao not support yet."
fi
