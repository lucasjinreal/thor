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
// Created by jintian on 18-1-11.
//


#include "../include/dog.h"
#include "thor/os.h"


Dog::Dog(string name) {
    LOG(INFO) << "Building your dog...";
    this->_name = name;
}

Dog::~Dog() {

}

vector<string> Dog::getAbilities() {
    vector<string> abilities;
    abilities.push_back("Eat");
    abilities.push_back("WelcomeHome");
    abilities.push_back("Sleep");
    abilities.push_back("Running");
    return abilities;
}

string Dog::getName() {
    return _name;
}
