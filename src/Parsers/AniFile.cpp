#include <Parsers/AniFile.h>

using namespace Sourcehold;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

/* RIFF header */
struct AniFile::RiffHeader {
	uint32_t magic;
	uint32_t filesize;
	uint32_t type;
};

/* Chunk header */
struct AniFile::RiffChunk {
	uint32_t id;
	uint32_t size;
};

AniFile::AniFile(std::shared_ptr<Rendering::Renderer> rend) : Parser() {
}

AniFile::AniFile(const AniFile &other) {
}

AniFile::AniFile(std::shared_ptr<Rendering::Renderer> rend, boost::filesystem::path path) : Parser() {
	this->LoadFromDisk(path);
}

AniFile::~AniFile() {
	
}

bool AniFile::LoadFromDisk(boost::filesystem::path path) {
    if(!Parser::Open(path.string(), std::ios::binary | std::ifstream::in)) {
        Logger::error("PARSERS")  << "Unable to load ani file '" << path.string() << "' from data folder!" << std::endl;
        return false;
    }
	
	RiffHeader header;
	if(
		!Parser::GetData(&header, sizeof(RiffHeader)) || /* IO error */
		header.magic != 1179011410 || /* Wrong magic id */
		header.filesize != Parser::GetLength() || /* Wrong filesize encoded */
		header.type != 1313817409 /* Should be 'ACON' */
	) {
        Logger::error("PARSERS")  << "Ani file error!" << std::endl;
		Parser::Close();
        return false;
    }
	
	return ParseChunks();
}

bool AniFile::ParseChunks() {
	while(Parser::Ok()) {
		RiffChunk header;
		Parser::GetData(&header, sizeof(RiffChunk));
		
		std::string id((const char*)&header.id, sizeof(uint32_t));
		size_t bytes = header.size % 2 == 0 ? header.size : header.size + 1;

		if(id == "anih") {
			/* TODO */
			Parser::SeekG(Parser::Tell() + bytes);
		}else if(id == "rate") {
			/* TODO */
			Parser::SeekG(Parser::Tell() + bytes);
		}else if(id == "seq ") {
			/* TODO */
			Parser::SeekG(Parser::Tell() + bytes);
		}else if(id == "LIST") {
			/* TODO */
			Parser::SeekG(Parser::Tell() + bytes);
		}else {
			Parser::SeekG(Parser::Tell() + bytes);
		}
	}
	
	Parser::Close();
	return true;
}
