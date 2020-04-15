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
/**
 * @file
 * 
 * 
 * Some macros which may useful when constructing C++ codes
 */

#ifndef SRC_MACRO_H_
#define SRC_MACRO_H_

#include <iomanip>
#include <iostream>

#include <libgen.h>  // basename
#include <iostream>  // cerr/cout

#define DISALLOW_COPY_AND_ASSIGN(classname) \
 private:                                   \
  classname(const classname &);             \
  classname &operator=(const classname &);

#define DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
 private:                                         \
  classname();                                    \
  DISALLOW_COPY_AND_ASSIGN(classname);

#define DECLARE_SINGLETON(classname)        \
 public:                                    \
  static classname *instance() {            \
    static classname instance;              \
    return &instance;                       \
  }                                         \
  DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
 private:



///////////////// micros from x start //////////////////////
#define ClearScreen() printf("\033[H\033[J")
#define PRINT_COLOR_RED "\033[22;31m"
#define PRINT_COLOR_GREEN "\033[22;32m"
#define PRINT_COLOR_YELLOW "\033[22;33m"
#define PRINT_COLOR_GRAY "\033[22;90m"
#define PRINT_COLOR_BLUE "\033[22;34m"
#define PRINT_COLOR_MAGENTA "\033[22;35m"
#define PRINT_COLOR_CYAN "\033[22;36m"
#define PRINT_COLOR_RESET "\033[0m"
#define PRINT_CLEAR_SCREEN "\033[H\033[J";

// inFO, DeBug, WaRn, ERror
#define FO(__O__) std::cout << PRINT_COLOR_YELLOW " INFO: " << PRINT_COLOR_RESET << __O__ << std::endl
#define DB(__O__)                                                                                    \
  std::cout << PRINT_COLOR_GRAY << "  DEBUG " << basename((char*)__FILE__) << ":" << __LINE__ << " " \
            << PRINT_COLOR_RESET << __O__ << std::endl
#define WR(__O__) std::cout << PRINT_COLOR_RED "WARN: " << PRINT_COLOR_RESET << __O__ << std::endl
#define ER(__O__) std::cerr << PRINT_COLOR_RED "ERROR: " << PRINT_COLOR_RESET << __O__ << std::endl

#define FO_EVERY_N(n, arg)                                                                \
  {                                                                                       \
    static int PRINT_OCCURRENCES_MOD_N = 0;                                               \
    if (++PRINT_OCCURRENCES_MOD_N > n) PRINT_OCCURRENCES_MOD_N -= n;                      \
    if (PRINT_OCCURRENCES_MOD_N == 1)                                                     \
      std::cout << PRINT_COLOR_YELLOW " INFO: " << PRINT_COLOR_RESET << arg << std::endl; \
  }

#define DB_EVERY_N(n, arg)                                                                               \
  {                                                                                                      \
    static int PRINT_OCCURRENCES_MOD_N = 0;                                                              \
    if (++PRINT_OCCURRENCES_MOD_N > n) PRINT_OCCURRENCES_MOD_N -= n;                                     \
    if (PRINT_OCCURRENCES_MOD_N == 1)                                                                    \
      std::cout << PRINT_COLOR_GRAY << "  DEBUG " << basename((char*)__FILE__) << ":" << __LINE__ << " " \
                << PRINT_COLOR_RESET << arg << std::endl;                                                \
  }

#define WR_EVERY_N(n, arg)                                                                                            \
  {                                                                                                                   \
    static int PRINT_OCCURRENCES_MOD_N = 0;                                                                           \
    if (++PRINT_OCCURRENCES_MOD_N > n) PRINT_OCCURRENCES_MOD_N -= n;                                                  \
    if (PRINT_OCCURRENCES_MOD_N == 1) std::cerr << PRINT_COLOR_RED "WARN: " << PRINT_COLOR_RESET << arg << std::endl; \
  }

#define ER_EVERY_N(n, arg)                                                                                             \
  {                                                                                                                    \
    static int PRINT_OCCURRENCES_MOD_N = 0;                                                                            \
    if (++PRINT_OCCURRENCES_MOD_N > n) PRINT_OCCURRENCES_MOD_N -= n;                                                   \
    if (PRINT_OCCURRENCES_MOD_N == 1) std::cerr << PRINT_COLOR_RED "ERROR: " << PRINT_COLOR_RESET << arg << std::endl; \
  }

// #ifdef NDEBUG
#define PRINT_RED(__O__) std::cout << PRINT_COLOR_RED << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_GREEN(__O__) std::cout << PRINT_COLOR_GREEN << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_YELLOW(__O__) std::cout << PRINT_COLOR_YELLOW << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_GRAY(__O__) std::cout << PRINT_COLOR_GRAY << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_BLUE(__O__) std::cout << PRINT_COLOR_BLUE << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_MAGENTA(__O__) std::cout << PRINT_COLOR_MAGENTA << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_CYAN(__O__) std::cout << PRINT_COLOR_CYAN << __O__ << PRINT_COLOR_RESET << std::endl
#define PRINT_WHITE(__O__) std::cout << __O__ << std::endl
/*
#else
#define PRINT_RED(__O__) \
  {}
#define PRINT_GREEN(__O__) \
  {}
#define PRINT_YELLOW(__O__) \
  {}
#define PRINT_GRAY(__O__) \
  {}
#define PRINT_BLUE(__O__) \
  {}
#define PRINT_MAGENTA(__O__) \
  {}
#define PRINT_CYAN(__O__) \
  {}
#define PRINT_WHITE(__O__) \
  {}
#endif
*/


/////////////////////// micros from x stop ///////////////////////////



#endif  // SRC_MACRO_H_