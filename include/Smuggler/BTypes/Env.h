#ifndef FLAGGER_ENV_H
#define FLAGGER_ENV_H

namespace VEnvironment {

#ifdef _WIN32
    #define NewLine "\r\n"
#elif defined macintosh // OS 9
    #define NewLine "\r"
#else
    #define NewLine "\n" // Mac OS X uses \n
#endif

}

#endif
