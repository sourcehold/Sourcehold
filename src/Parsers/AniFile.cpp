#include <Parsers/AniFile.h>

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

AniFile::AniFile(std::shared_ptr<Rendering::Renderer> rend, const std::string &path) : Parser() {
	this->LoadFromDisk(path);
}

AniFile::~AniFile() {
	
}

bool AniFile::LoadFromDisk(const std::string &path) {
	if(!Parser::Open(path, std::ios::binary)) {
        Logger::error("PARSERS")  << "Unable to load ani file '" << path << "' from data folder!" << std::endl;
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
		
		std::cout << id << std::endl;

		if(id == "anih") {
			/* TODO */
			Parser::Seek(Parser::Tell() + bytes);
		}else if(id == "rate") {
			/* TODO */
			Parser::Seek(Parser::Tell() + bytes);
		}else if(id == "seq ") {
			/* TODO */
			Parser::Seek(Parser::Tell() + bytes);
		}else if(id == "LIST") {
			/* TODO */
			Parser::Seek(Parser::Tell() + bytes);
		}else {
			Parser::Seek(Parser::Tell() + bytes);
		}
	}
	
	Parser::Close();
	return true;
}
