#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <System/Logger.h>

#include <Parsers/Parser.h>

namespace Sourcehold
{
	namespace Parsers
	{
		/*
		 * Animated cursor parser
		 */
		class AniFile : public Parser
		{
			public:
				AniFile();
				AniFile(const AniFile&);
				~AniFile();
				
				bool LoadFromDisk(const std::string &path);
			protected:
				bool ParseChunks();
				struct RiffHeader;
				struct RiffChunk;
		};
	}
}
