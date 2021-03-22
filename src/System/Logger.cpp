#include "System/Logger.h"

using namespace Sourcehold::System;

#ifdef SOURCEHOLD_ANDROID
  #include <android/log.h>

// https://stackoverflow.com/questions/8870174/is-stdcout-usable-in-android-ndk/8870278#8870278
static class AndroidBuf : public std::streambuf {
 public:
  enum { bufsize = 128 };  // ... or some other suitable buffer size
  AndroidBuf() {
    this->setp(buffer, buffer + bufsize - 1);
  }

 private:
  int overflow(int c) {
    if (c == traits_type::eof()) {
      *this->pptr() = traits_type::to_char_type(c);
      this->sbumpc();
    }
    return this->sync() ? traits_type::eof() : traits_type::not_eof(c);
  }

  int sync() {
    int rc = 0;
    if (this->pbase() != this->pptr()) {
      char writebuf[bufsize + 1];
      memcpy(writebuf, this->pbase(), this->pptr() - this->pbase());
      writebuf[this->pptr() - this->pbase()] = '\0';

      rc = __android_log_write(ANDROID_LOG_INFO, "std", writebuf) > 0;
      this->setp(buffer, buffer + bufsize - 1);
    }
    return rc;
  }

  char buffer[bufsize];
} androidbuf;
std::ostream stream(&androidbuf);
#endif

static bool coloredOutput = false;

Logger::Logger() {
}

Logger::~Logger() {
}

std::ostream& Logger::log(LogType type, const std::string& subsystem) {
  std::string msg = "";
  if (coloredOutput) {
    switch (type) {
      case LogType::ERROR:
        msg = "\033[1;31m[ " + subsystem + " ]\033[0m -> ";
        break;
      case LogType::WARNING:
        msg = "\033[1;33m[ " + subsystem + " ]\033[0m -> ";
        break;
      case LogType::MESSAGE:
        msg = "\033[1;34m[ " + subsystem + " ]\033[0m -> ";
        break;
      case LogType::SUCCESS:
        msg = "\033[1;32m[ " + subsystem + " ]\033[0m -> ";
        break;
      default:
        break;
    }
  }
  else {
    msg = "[ " + subsystem + " ] -> ";
  }
#ifdef SOURCEHOLD_ANDROID
  return stream << msg;
#else
  return std::cout << msg;
#endif
}

std::ostream& Logger::error(Subsystem subsystem) {
  return log(LogType::ERROR, SubsystemName(subsystem));
}

std::ostream& Logger::warning(Subsystem subsystem) {
  return log(LogType::WARNING, SubsystemName(subsystem));
}

std::ostream& Logger::message(Subsystem subsystem) {
  return log(LogType::MESSAGE, SubsystemName(subsystem));
}

std::ostream& Logger::success(Subsystem subsystem) {
  return log(LogType::SUCCESS, SubsystemName(subsystem));
}

void Logger::SetColorOutput(bool a) {
  coloredOutput = a;
}

bool Logger::GetColorOutput() {
  return coloredOutput;
}

std::string Logger::SubsystemName(Subsystem sys) {
  std::string s;

  switch (sys) {
    case AUDIO:
      s = "AUDIO";
      break;
    case EVENTS:
      s = "EVENTS";
      break;
    case GUI:
      s = "GUI";
      break;
    case PARSERS:
      s = "PARSERS";
      break;
    case RENDERING:
      s = "RENDERING";
      break;
    case GAME:
      s = "GAME";
      break;
    case ECS:
      s = "ECS";
      break;
    case SOURCEHOLD:
    default:
      s = "UNKNOWN";
      break;
  }

  return s;
}
