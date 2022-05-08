#!/bin/sh
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

case "$(uname -s)" in

Darwin)
    echo 'macOS'
    rm include/thor/proto/*.pb.cc
    rm include/thor/proto/*.pb.h
    # brew install protobuf

    ;;

Linux)
    echo 'Linux'
    rm include/proto/*.pb.cc
    rm include/proto/*.pb.h

    sudo apt install -y libfreetype6-dev
    sudo apt install -y libcurl4-openssl-dev
    sudo apt install -y libeigen3-dev
    sudo apt-get install -y libprotobuf-dev
    sudo apt install -y protobuf-compiler
    ;;

CYGWIN* | MINGW32* | MSYS* | MINGW*)
    echo 'MS Windows'
    ;;

# Add here more strings to compare
# See correspondence table at the bottom of this answer
*)
    echo 'Other OS, this might not supported, if you got issues, fire it on github.'
    ;;
esac

mkdir build
cd build
# cmake -DUSE_PROTOBUF=ON ..
cmake -DUSE_PROTOBUF=OFF ..
make -j2
sudo make install

echo "thor installed."
