#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace Sourcehold
{
    namespace Audio
    {
        /*
         * Init OpenAL context, called once at the start of
         * the app and is required by other audio stuff 
         */
        bool InitOpenAL();

        /*
         * Destroy the previously created OpenAL context
         */
        void DestroyOpenAL();

        /*
         * Print an error reported by OpenAL, doesn't do
         * anything if no error happened
         */
        void PrintError();
    }
}

