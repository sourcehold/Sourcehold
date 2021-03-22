#pragma once

#include <cinttypes>

namespace Sourcehold {
namespace Parsers {
/* RIFF header */
struct RiffHeader {
  uint32_t magic;
  uint32_t filesize;
  uint32_t type;
};

/* Chunk header */
struct RiffChunk {
  uint32_t id;
  uint32_t size;
};

/* Wav format chunk */
struct WavFmt {
  uint32_t id;
  uint32_t size;
  uint16_t fmt;
  uint16_t chn;
  uint32_t smp;
  uint32_t byte;
  uint16_t align;
  uint16_t bps;
};

struct WavFact {
  uint32_t id;
  uint32_t size;
  uint32_t smp;
};
}  // namespace Parsers
}  // namespace Sourcehold
