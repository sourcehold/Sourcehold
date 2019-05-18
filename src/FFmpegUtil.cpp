#include <FFmpegUtil.h>
#include <System/Logger.h>

using namespace Sourcehold;
using namespace System;

static AVInputFormat *bink_input;
bool Game::InitAvcodec() {
	av_log_set_level(AV_LOG_ERROR);
	bink_input = av_find_input_format("bink");
	if (!bink_input) {
		Logger::error("RENDERING") << "Unable to find libavcodec input format 'bink'!" << std::endl;
		return false;
	}

	return true;
}

void Game::DestroyAvcodec() {

}

AVInputFormat *Game::GetAVInputFormat() {
	return bink_input;
}
