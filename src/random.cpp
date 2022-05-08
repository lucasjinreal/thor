/*
 * Copyright (c) 2020 Fagang Jin.
 *
 * This file is part of thor
 * (see manaai.cn).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
//
// Created by jintian on 7/12/17.
//

// provide multi random value

#include <cstdlib>
#include <stdexcept>

#include "time.h"
#include "thor/random.h"

using namespace std;

int thor::random::randint(int a, int b){

    if (a >= b){
        throw std::invalid_argument("randint argument a must less than b.");
    }
    srand((unsigned) time(NULL));
    return a + rand() % (a + b -1);
}

float thor::random::randnorm(){
    // return a float value random in 0-1
    return rand();
}


void thor::random::kick_it_random() {
    srand((unsigned int) time(0));
}