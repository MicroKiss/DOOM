
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//-----------------------------------------------------------------------------

#include "doomdef.hpp"

#include <string.h>

#include "Miscellaneous/argv.hpp"
#include "Doom/main.hpp"
#include "SystemInterface/system.hpp"
#include "SystemInterface/video.hpp"
#include "Renderer/video.hpp"
#include "SystemInterface/main_test.hpp"

int main(int argc, char **argv)
{
    int i;

    myargc = argc;
    myargv = argv;

#ifdef DOOM_DEBUG
    int test_result = TestMain();
    if (test_result >= 0)
        return test_result;
#endif

    D_DoomMain();

    I_Quit();
    return 0;
}
