#include <fstream>
#include <streambuf>
#include <locale>
#include <codecvt>
#include <cinttypes>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "Parsers/HlpFile.h"
#include "System/Logger.h"
#include "GameManager.h"

using namespace Sourcehold;
using namespace Game;
using namespace Parsers;
using namespace System;

static std::vector<HlpSection> _sections;

int HlpSection::GetIntArg(size_t index)
{
    try {
        return std::stoi(args.at(index));
    } catch(const std::invalid_argument&) {
        return -1;
    } catch(const std::out_of_range&) {
        return -1;
    }
}

std::wstring HlpSection::GetStrArg(size_t index)
{
    try {
        return args.at(index);
    } catch(const std::out_of_range&) {
        return std::wstring();
    }
}

bool Parsers::LoadStrongholdHlp()
{
    auto path = GetDirectory() / "help/stronghold_help.hlp";
    std::wifstream ws(
        path.string(),
        std::ios::binary
        );
    if(!ws.is_open()) {
        Logger::error(PARSERS) << "Unable to load help file from " << path.string() << std::endl;
        return false;
    }

    ws.ignore(2); // magic number
    ws.imbue(std::locale(ws.getloc(),new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

    std::wstring fb(
        (std::istreambuf_iterator<wchar_t>(ws)),
        std::istreambuf_iterator<wchar_t>()
        );

    ws.close();

    const std::string matchAssign = "= ";

    HlpSection currentSec;
    for(std::wstring::iterator it = fb.begin(); it != fb.end(); ++it) {
        if(*it == L'<') {
            std::wstring::iterator endOfContents = fb.begin() + fb.find(L'>', it - fb.begin());

            // everything in-between < and >
            std::wstring contents(it+1, endOfContents);

            // the identifier (i.e. 'SECTION')
            bool isEndOfBlock = false;
            std::vector<std::wstring> cts;
            std::wstring ident = boost::algorithm::split(cts, contents, boost::algorithm::is_any_of(matchAssign)).at(0);
            if(ident.at(0) == L'\\') {
                isEndOfBlock = true;
                ident = ident.substr(1);
            }

            /**
             * This is the bodged part:
             * We don't bother parsing arguments intelligently, but
             * expect certain arguments for the type of command, for
             * example 'FONT' is always followed by an int.
             */
            if(ident == L"PIC") {
                // TODO: case <PIC=5, LEFT> doesn't match pos!!!!!!
                boost::algorithm::split(cts, contents, boost::algorithm::is_any_of(",= "));
                std::wstring num = cts.at(1);
                std::wstring pos = cts.at(2);

                HlpSection sec;
                sec.type = SectionType::PIC;
                sec.args.push_back(num);
                sec.args.push_back(pos);

                currentSec.children.push_back(sec);
            }else if(ident == L"FONT") {
                boost::algorithm::split(cts, contents, boost::algorithm::is_any_of(matchAssign));
                std::wstring num = cts.at(1);

                HlpSection sec;
                sec.type = SectionType::FONT;
                sec.args.push_back(num);

                currentSec.children.push_back(sec);
            }else if(ident == L"COLOUR") {
                boost::algorithm::split(cts, contents, boost::algorithm::is_any_of(matchAssign));
                std::wstring num = cts.at(1);

                HlpSection sec;
                sec.type = SectionType::COLOUR;
                sec.args.push_back(num);

                currentSec.children.push_back(sec);
            }else if(ident == L"SECTION") {
                if(isEndOfBlock) {
                    // end of section reached, push and reset
                    _sections.push_back(currentSec);

                    currentSec.children.clear();
                    currentSec.args.clear();
                }else {
                    // new section created
                    std::wstring name = boost::algorithm::split(cts, contents, boost::algorithm::is_any_of("\"")).at(1);

                    currentSec.name = name;
                    currentSec.type = SectionType::SECTION;
                }
            }else if(ident == L"LOADPIC") {
                std::wstring name = boost::algorithm::split(cts, contents, boost::algorithm::is_any_of("\"")).at(1);

                HlpSection sec;
                sec.name = name;
                sec.type = SectionType::LOADPIC;

                currentSec.children.push_back(sec);
            }else if(ident == L"HEADER") {
                HlpSection sec;
                sec.type = isEndOfBlock ? SectionType::ENDHEADER : SectionType::HEADER;
                currentSec.children.push_back(sec);
            }else if(ident == L"BODY") {
                HlpSection sec;
                sec.type = isEndOfBlock ? SectionType::ENDBODY : SectionType::BODY;
                currentSec.children.push_back(sec);
            }else if(ident == L"INCLUDE") {
                std::wstring name = boost::algorithm::split(cts, contents, boost::algorithm::is_any_of("\"")).at(1);

                HlpSection sec;
                sec.name = name;
                sec.type = SectionType::INCLUDE;

                currentSec.children.push_back(sec);
            }else if(ident == L"NEWPARAGRAPH") {
                HlpSection sec;
                sec.type = SectionType::NEWPARAGRAPH;
                currentSec.children.push_back(sec);
            }else if(ident == L"LINK") {
                HlpSection sec;
                sec.type = isEndOfBlock ? SectionType::ENDLINK : SectionType::LINK;
                currentSec.children.push_back(sec);
            }else if(ident == L"CENTRE") {
                HlpSection sec;
                sec.type = isEndOfBlock ? SectionType::ENDCENTRE : SectionType::CENTRE;
                currentSec.children.push_back(sec);
            }else {
                Logger::warning(PARSERS) << "Unknown comand in help file at pos " << it - fb.begin() << std::endl;
            }
        }
    }

    return true;
}

void Parsers::UnloadStrongholdHlp()
{
    _sections.clear();
}

HlpSection *Parsers::GetHlpSection(std::wstring name)
{
    // TODO: speed up
    for(std::vector<HlpSection>::iterator it = _sections.begin(); it != _sections.end(); ++it) {
        if(it->name == name) return &(*it);
    }
}

