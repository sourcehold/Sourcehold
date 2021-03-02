#ifndef FILE_STREAM_H_
#define FILE_STREAM_H_
#include <algorithm>
#include <array>
#include <cinttypes>
#include <codecvt>
#include <fstream>
#include <iterator>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

#include "FileSystem.h"

// std::fstream wrapper.
// Handles read and write operations on a file.
// Throws on failure
class FileStream {
 public:
  using Path = ghc::filesystem::path;
  using OpenMode = std::ios_base::openmode;
  using PosType = std::fstream::pos_type;
  // Creates and opens a fstream at path with mode
  FileStream(const Path &path, OpenMode mode);
  // Closes the fstream
  ~FileStream();
  // The following functions resemble fstream counterparts
  void Skip(PosType n) noexcept;
  void SeekG(PosType pos) noexcept;
  void SeekP(PosType pos) noexcept;
  PosType TellG() noexcept;

  // Reads T from stream.
  // reinterpret_cast from T to char*
  template <typename T>
  T Get();
  template <typename T, bool reversed = false>
  std::vector<T> Get(size_t n);
  template <typename T, size_t n, bool reversed = false>
  std::array<T, n> Get();
  template <typename T>
  std::vector<T> GetToEOF();

  std::string GetLine();

  // Writes T to stream
  // reinterpret_cast from T to char*
  template <typename T>
  void Write(T data);
  template <typename T, size_t n>
  void Write(const std::array<T, n> &data);

 private:
  std::fstream stream_;
};

template <typename T>
T FileStream::Get() {
  T data;
  stream_.read(reinterpret_cast<char *>(&data), sizeof(data));
  // Logging
  if (stream_.bad()) {
    throw std::runtime_error("I/O Error: Reading from file");
  }
  return data;
}

template <typename T, bool reversed>
std::vector<T> FileStream::Get(size_t n) {
  std::vector<T> data(n);

  reversed
      ? std::generate(data.rbegin(), data.rend(), [&]() { return Get<T>(); })
      : std::generate(data.begin(), data.end(), [&]() { return Get<T>(); });
  return data;
}
template <typename T, size_t n, bool reversed>
std::array<T, n> FileStream::Get() {
  std::array<T, n> data;

  reversed
      ? std::generate(data.rbegin(), data.rend(), [&]() { return Get<T>(); })
      : std::generate(data.begin(), data.end(), [&]() { return Get<T>(); });
  return data;
}
template <typename T>
std::vector<T> FileStream::GetToEOF() {
  std::vector<T> result;
  std::transform(std::istream_iterator<uint8_t>(stream_),  //
                 std::istream_iterator<uint8_t>(),         //
                 std::back_inserter(result),               //
                 Get<T>());
  return result;
}

template <typename T>
void FileStream::Write(T data) {
  stream_.write(reinterpret_cast<char *>(&data), sizeof(T));
  // Logging
  if (stream_.bad()) {
    throw std::runtime_error("I/O Error: Writing to file");
  }
}
template <typename T, size_t n>
void FileStream::Write(const std::array<T, n> &data) {
  for (auto &i : data) {
    Write<T>(i);
  }
}
#endif  // FILE_STREAM_H_
