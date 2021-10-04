//-----------------------------------------------------------------------------
// Copyright (c) 2013-2018 Benjamin Buch
//
// https://github.com/bebuch/tar
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#pragma once

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "thor/io.h"

using std::integer_sequence;
using std::make_unique;
using std::move;
using std::stol;

namespace thor {
namespace tar {

using index_t = std::string::difference_type;

template <index_t... I>
using index_sequence_t = std::integer_sequence<index_t, I...>;

using field_size_t = index_sequence_t<100, 8, 8, 8, 12, 12, 8, 1, 100, 6, 2, 32,
                                      32, 8, 8, 155, 12>;

template <typename IntegerSequence, index_t X>
struct get_integer;

template <index_t H, index_t... I, index_t X>
struct get_integer<index_sequence_t<H, I...>, X> {
  static constexpr index_t value =
      get_integer<index_sequence_t<I...>, X - 1>::value;
};

template <index_t H, index_t... I>
struct get_integer<index_sequence_t<H, I...>, index_t(0)> {
  static constexpr index_t value = H;
};

template <index_t X>
struct field_size {
  static constexpr index_t value = get_integer<field_size_t, X>::value;
};

template <typename IntegerSequence, index_t X>
struct get_integer_start;

template <index_t H, index_t... I, index_t X>
struct get_integer_start<index_sequence_t<H, I...>, X> {
  static constexpr index_t value =
      get_integer_start<index_sequence_t<I...>, X - 1>::value + H;
};

template <index_t H, index_t... I>
struct get_integer_start<index_sequence_t<H, I...>, index_t(0)> {
  static constexpr index_t value = 0;
};

template <index_t X>
struct field_start {
  static constexpr index_t value = get_integer_start<field_size_t, X>::value;
};

struct field_name {
  enum values {
    name,
    mode,
    uid,
    gid,
    size,
    mtime,
    checksum,
    typeflag,
    linkname,
    magic,
    version,
    uname,
    gname,
    devmajor,
    devminor,
    prefix,
    pad
  };
};

template <index_t FieldName, typename Container>
void write(std::array<char, 512>& buffer, Container const& container) {
  static constexpr auto start = field_start<FieldName>::value;
  static constexpr auto size = field_size<FieldName>::value;

  auto const begin = container.begin();
  auto const end = container.end();

  if (end - begin > size) {
    throw std::runtime_error(
        thor::io::make_string("Tar: field data to long: [", FieldName,
                              "] is: ", end - begin, ", max: ", size));
  }

  std::copy(begin, end, buffer.begin() + start);
  std::fill(buffer.begin() + start + (end - begin),
            buffer.begin() + start + size, 0);
}

template <index_t FieldName>
std::string read(std::array<char, 512> const& buffer) {
  static constexpr auto start = field_start<FieldName>::value;
  static constexpr auto size = field_size<FieldName>::value;

  return std::string(buffer.begin() + start, buffer.begin() + start + size);
}

constexpr std::array<char, field_size<field_name::checksum>::value>
    empty_checksum{{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

constexpr std::array<char, 5> magic{{'u', 's', 't', 'a', 'r'}};
constexpr std::array<char, 6> mode{{'0', '0', '0', '6', '4', '4'}};
constexpr std::array<char, 1> typeflag{{'0'}};

inline std::string calc_checksum(std::array<char, 512> buffer) {
  write<field_name::checksum>(buffer, empty_checksum);

  unsigned sum = 0;
  for (unsigned i : buffer) sum += i;

  std::ostringstream os;
  os << std::oct << std::setfill('0') << std::setw(6) << sum << '\0' << ' ';

  return os.str();
}

inline std::array<char, 512> make_posix_header(std::string const& name,
                                               std::size_t size) {
  std::array<char, 512> buffer{};

  std::ostringstream os;
  os << std::oct << std::setfill('0') << std::setw(11) << std::time(nullptr);
  std::string mtime = os.str();

  write<field_name::magic>(buffer, magic);
  write<field_name::mode>(buffer, mode);
  write<field_name::mtime>(buffer, mtime);
  write<field_name::typeflag>(buffer, typeflag);

  // Set filename
  if (name.size() == 0) {
    throw std::runtime_error("Tar: filename is empty");
  }

  if (name.size() >= 100) {
    throw std::runtime_error("Tar: filename larger than 99 charakters");
  }

  write<field_name::name>(buffer, name);

  // Set size
  os.str("");
  os << std::oct << std::setfill('0') << std::setw(11) << size;
  write<field_name::size>(buffer, os.str());

  // Set checksum
  write<field_name::checksum>(buffer, calc_checksum(buffer));

  return buffer;
}

inline std::string cut_null(std::string const& data) {
  return data.substr(0, data.find('\0'));
}

inline std::tuple<std::string, std::size_t> read_posix_header(
    std::array<char, 512> const& buffer) {
  auto const checksum = read<field_name::checksum>(buffer);
  auto const magic = cut_null(read<field_name::magic>(buffer));
  auto const size = read<field_name::size>(buffer);
  auto const filename = cut_null(read<field_name::name>(buffer));

//   if (magic != "ustar") {
//     throw std::runtime_error(
//         "Tar: loaded file without magic 'ustar', magic is: '" +
//         thor::io::mask_non_print(magic) + "'");
    
//   }

//   if (checksum != calc_checksum(buffer)) {
//     throw std::runtime_error("Tar: loaded file with wrong checksum");
//   }

  return std::make_tuple(std::move(filename),
                         static_cast<std::size_t>(std::stol(size, 0, 8)));
}

/// \brief Write a simple tar file
class tar_writer {
 public:
  explicit tar_writer(std::string const& filename)
      : outfile_(new std::ofstream(filename.c_str(),
                                   std::ios_base::out | std::ios_base::binary)),
        out_(*outfile_) {}

  explicit tar_writer(std::ostream& out) : out_(out) {}

  ~tar_writer() {
    static char const dummy[1024] = {0};
    out_.write(dummy, 1024);
  }

  void write(std::string const& filename, char const* content,
             std::size_t size) {
    write(
        filename, [&](std::ostream& os) { os.write(content, size); }, size);
  }

  void write(std::string const& filename, std::string const& content) {
    write(filename, content.data(), content.size());
  }

  void write(std::string const& filename,
             std::function<void(std::ostream&)> const& writer) {
    std::ostringstream os(std::ios_base::out | std::ios_base::binary);
    writer(os);
    write(filename, os.str());
  }

  void write(std::string const& filename,
             std::function<void(std::ostream&)> const& writer,
             std::size_t size) {
    if (!filenames_.emplace(filename).second) {
      throw std::runtime_error("Duplicate filename in tar-file: " + filename);
    }

    auto const header = thor::tar::make_posix_header(filename, size);

    std::size_t end_record_bytes = (512 - (size % 512)) % 512;
    std::vector<char> buffer(end_record_bytes);

    auto start = out_.tellp();
    out_.write(header.data(), header.size());

    writer(out_);
    auto wrote_size = static_cast<std::size_t>(out_.tellp() - start) - 512;
    if (wrote_size != size) {
      out_.seekp(start);
      throw std::runtime_error(thor::io::make_string(
          "While writing '", filename, "' to tar-file: Writer function wrote ",
          wrote_size, " bytes, but ", size, " where expected"));
    }

    out_.write(buffer.data(), buffer.size());
  }

 private:
  /// \brief Output file, if the filename constructor have been called
  std::unique_ptr<std::ofstream> outfile_;

  /// \brief Output stream
  std::ostream& out_;

  /// \brief No filename duplicates
  std::set<std::string> filenames_;
};

/// \brief Write a simple tar file
class tar_reader {
 public:
  explicit tar_reader(std::string const& filename)
      : isptr_(std::make_unique<std::ifstream>(
            filename.c_str(), std::ios_base::in | std::ios_base::binary)),
        is_(*isptr_.get()) {
    init();
  }

  explicit tar_reader(std::istream& is) : is_(is) { init(); }

  inline void list_files() {
    for (auto& mp : files_) {
      std::cout << mp.first << std::endl;
    }
  }

  std::istream& get(std::string const& filename) {
    auto iter = files_.find(filename);
    if (iter == files_.end()) {
      throw std::runtime_error("Filename-entry not fount in tar-file: " +
                               filename);
    }

    iter->second.seekg(0);
    return iter->second;
  }

 private:
  void init() {
    static constexpr std::array<char, 512> empty_buffer{};

    std::array<char, 512> buffer;
    while (is_) {
      is_.read(buffer.data(), 512);

      if (buffer == empty_buffer) {
        is_.read(buffer.data(), 512);
        if (buffer != empty_buffer || !is_) {
          throw std::runtime_error("Corrupt tar-file.");
        }
        break;
      }

      std::string filename;
      std::size_t size;
      std::tie(filename, size) = thor::tar::read_posix_header(buffer);

      auto result = files_.emplace(
          std::piecewise_construct, std::forward_as_tuple(filename),
          std::forward_as_tuple(is_.rdbuf(), is_.tellg(), size));
      if (!result.second) {
        throw std::runtime_error(
            "Duplicate filename-entry while reading tar-file: " + filename);
      }

      std::streampos file_size_in_tar = size + (512 - (size % 512)) % 512;
      is_.seekg(is_.tellg() + file_size_in_tar);

      if (!is_) {
        throw std::runtime_error("Tar filename-entry with illegal size: " +
                                 filename);
      }
    }
  }

  /// \brief Stream if read via filename
  std::unique_ptr<std::ifstream> isptr_;

  /// \brief Input stream of the tar-file
  std::istream& is_;

  /// \brief Map of filenames and contents
  std::map<std::string, thor::io::isubstream> files_;
};

}  // namespace tar

}  // namespace thor
