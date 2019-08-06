//
// Created by jintian on 19-5-23.
//

#ifndef THOR_INCLUDE_FILE_H_
#define THOR_INCLUDE_FILE_H_

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#ifdef USE_PROTOBUF
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
#endif

/**
 *
 *
 * A set of file utils methods
 *
 *
 */

namespace thor {
namespace file {

#ifdef USE_PROTOBUF

template<typename MessageType>
bool SetProtoToASCIIFile(const MessageType &message, int file_descriptor) {
  using google::protobuf::TextFormat;
  using google::protobuf::io::FileOutputStream;
  using google::protobuf::io::ZeroCopyOutputStream;
  if (file_descriptor < 0) {
    return false;
  }
  ZeroCopyOutputStream *output = new FileOutputStream(file_descriptor);
  bool success = TextFormat::Print(message, output);
  delete output;
  close(file_descriptor);
  return success;
}

template<typename MessageType>
bool SetProtoToASCIIFile(const MessageType &message,
                         const std::string &file_name) {
  int fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  if (fd < 0) {
    return false;
  }
  return SetProtoToASCIIFile(message, fd);
}

template<typename MessageType>
bool GetProtoFromASCIIFile(const std::string &file_name, MessageType *message) {
  using google::protobuf::TextFormat;
  using google::protobuf::io::FileInputStream;
  using google::protobuf::io::ZeroCopyInputStream;
  int file_descriptor = open(file_name.c_str(), O_RDONLY);
  if (file_descriptor < 0) {
    // Failed to open;
    return false;
  }

  ZeroCopyInputStream *input = new FileInputStream(file_descriptor);
  bool success = TextFormat::Parse(input, message);
  if (!success) {
  }
  delete input;
  close(file_descriptor);
  return success;
}

template<typename MessageType>
bool SetProtoToBinaryFile(const MessageType &message,
                          const std::string &file_name) {
  std::fstream output(file_name,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  return message.SerializeToOstream(&output);
}

template<typename MessageType>
bool GetProtoFromBinaryFile(const std::string &file_name,
                            MessageType *message) {
  std::fstream input(file_name, std::ios::in | std::ios::binary);
  if (!input.good()) {
    //AERROR << "Failed to open file " << file_name << " in binary mode.";
    return false;
  }
  if (!message->ParseFromIstream(&input)) {
    //AERROR << "Failed to parse file " << file_name << " as binary proto.";
    return false;
  }
  return true;
}

template<typename MessageType>
bool GetProtoFromFile(const std::string &file_name, MessageType *message) {
  return GetProtoFromASCIIFile(file_name, message) || GetProtoFromBinaryFile(file_name, message);
  //std::cout << "Called GetProtoFromFile" << std::endl;
  //return GetProtoFromASCIIFile(file_name, message);
}


#endif

}
}

#endif //THOR_INCLUDE_FILE_H_
