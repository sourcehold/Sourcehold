#include <fstream>
#include <streambuf>
#include <locale>
#include <codecvt>
#include <cinttypes>
#include <regex>
#include <boost/filesystem.hpp>

#include <Parsers/HlpFile.h>
#include <System/Logger.h>
#include <GameManager.h>

using namespace Sourcehold;
using namespace Game;
using namespace Parsers;
using namespace System;

bool Parsers::LoadStrongholdHlp()
{
	auto path = GetDirectory() / "help/stronghold_help.hlp";
	std::wifstream ws(
		path.string(),
		std::ios::binary
	);
	if(!ws.is_open()) {
		Logger::error("PARSERS") << "Unable to load help file from " << path.string() << std::endl;
		return false;
	}

	ws.ignore(2); // magic number
	ws.imbue(std::locale(ws.getloc(),new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

	std::wstring fb(
		(std::istreambuf_iterator<wchar_t>(ws)),
        std::istreambuf_iterator<wchar_t>()
	);

	ws.close();

	for(std::wstring::iterator it = fb.begin(); it != fb.end(); ++it) {
		if(*it == L'<') {

		}else if(*it == L'>') {

		}else {

		}
	}

	return true;
}

void Parsers::UnloadStrongholdHlp()
{
}
