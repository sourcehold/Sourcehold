#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#include <System/Logger.h>
#include <Rendering/Renderer.h>
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
				AniFile(std::shared_ptr<Rendering::Renderer> rend);
				AniFile(const AniFile&);
				AniFile(std::shared_ptr<Rendering::Renderer> rend, const std::string &path);
				~AniFile();
				
				bool LoadFromDisk(const std::string &path);
			protected:
				bool ParseChunks();
				struct RiffHeader;
				struct RiffChunk;
		};
	}
}
