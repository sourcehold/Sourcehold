#include "FileStream.h"

FileStream::FileStream(const Path& path, OpenMode mode) {
  stream_.open(path, mode);
  // Logging
  if (stream_.bad()) {
    const auto error = "failed to open file at " + path.string()  //
                       + " with mode " + std::to_string(mode);
    throw std::runtime_error(error);
  }
}
FileStream::~FileStream() { stream_.close(); }

void FileStream::Skip(PosType n) noexcept {
  stream_.seekg(n, std::ios_base::cur);
}

void FileStream::SeekG(PosType pos) noexcept {
  stream_.seekg(pos, std::ios_base::beg);
}

void FileStream::SeekP(PosType pos) noexcept {
  stream_.seekp(pos, std::ios_base::beg);
}

FileStream::PosType FileStream::TellG() noexcept { return stream_.tellg(); }

std::string FileStream::GetLine() {
  std::string line;
  std::getline(stream_, line);
  // Logging
  if (stream_.bad()) {
    throw std::runtime_error("I/O Error: Failed to read line from file");
  }
  return line;
}
