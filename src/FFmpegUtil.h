#pragma once

extern "C" {
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

namespace Sourcehold {
    namespace Game {
        /*
        * Init the avcodec context needed by the BinkVideo class
        */
        bool InitAvcodec();

        /*
        * Destroy a previously created avcodec context and free
        * associated resources
        */
        void DestroyAvcodec();

        AVInputFormat *GetAVInputFormat();
    }
}
