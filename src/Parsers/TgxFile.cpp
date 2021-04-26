#include "Parsers/TgxFile.h"
#include "System/Config.h"
#include "SDL/SurfaceLock.h"

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

struct TgxFile::TgxHeader {
  uint32_t width;
  uint32_t height;
};

TgxFile::TgxFile() {
}

TgxFile::TgxFile(ghc::filesystem::path path) {
  this->LoadFromDisk(path);
}

TgxFile::~TgxFile() {
  Unload();
}

bool TgxFile::LoadFromDisk(ghc::filesystem::path path) {
  if (!Parser::Open(path.string(), std::ifstream::in | std::ios::binary)) {
    Logger::error(PARSERS) << "Unable to open Tgx file '" << path << "'!"
                           << std::endl;
    return false;
  }

  TgxHeader header;
  header.width = GetDWord();
  header.height = GetDWord();

  /* Allocate image */
  Surface surf({static_cast<int>(header.width),  //
                static_cast<int>(header.height)});

  /* Calculate size */
  size_t size = Parser::GetLength() - Parser::Tell();

  char *buf = new char[size];
  Parser::GetData(buf, size);
  Parser::Close();

  /* Read image data */
  auto surf_lock = SDL::SurfaceScopedLock(surf);
  ReadTgx(surf, buf, size, 0, 0, nullptr);

  /* Convert to texture */
  Texture::AllocFromSurface(surf);

  delete[] buf;
  return true;
}

void TgxFile::Unload() {
}

void TgxFile::ReadTgx(Surface &tex, char *buf, size_t size, uint16_t offX,
                      uint16_t offY, uint16_t *pal, uint8_t color) {
  int32_t x = 0, y = 0;
  char *end = buf + size;

  while (buf < end) {
    /* Read token byte */
    uint8_t b = *(uint8_t *)buf;
    buf++;
    uint8_t len = (b & 0x1F) + 1;
    uint8_t flag = b >> 5;

    switch (flag) {
      case 0x00: {
        // pixel
        for (uint8_t i = 0; i < len; ++i, ++x) {
          uint16_t pixelColor;
          if (pal) {
            uint8_t index = *(uint8_t *)buf;
            buf++;
            pixelColor = pal[256 * color + index];
          }
          else {
            pixelColor = *reinterpret_cast<uint16_t *>(buf);
            buf += 2;
          }
          uint8_t r, g, b, a;
          ReadPixel(pixelColor, r, g, b, a);
          tex.Set({x + offX, y + offY}, {r, g, b, a});
        }
      } break;
      case 0x04: {
        // line break
        y++;
        x = 0;
      } break;
      case 0x02: {
        // repeated pixel
        uint16_t pixelColor;
        if (pal) {
          uint8_t index = *(uint8_t *)buf;
          buf++;
          pixelColor = pal[256 * color + index];
        }
        else {
          pixelColor = *reinterpret_cast<uint16_t *>(buf);
          buf += 2;
        }
        uint8_t r, g, b, a;

        ReadPixel(pixelColor, r, g, b, a);
        for (uint8_t i = 0; i < len; ++i, ++x) {
          tex.Set({x + offX, y + offY}, {r, g, b, 0xFF});
        }
      } break;
      case 0x01: {
        // transparent pixels
        for (uint8_t i = 0; i < len; i++, x++) {
          tex.Set({x + offX, y + offY}, {0, 0, 0, 0});
        }
      } break;
      default: {
        Logger::warning(PARSERS) << "Unknown token in tgx!" << std::endl;
        return;
      } break;
    }
  }
}

void TgxFile::ReadPixel(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b,
                        uint8_t &a) {
  a = 0xFF;
  r = ((pixel >> 10) & 0x1F) << 3;
  g = ((pixel >> 5) & 0x1F) << 3;
  b = (pixel & 0x1F) << 3;
}
