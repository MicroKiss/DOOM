#include <string.h>
#include "Miscellaneous/argv.hpp"
#ifdef _WIN32
#define strcasecmp _stricmp
#endif

int myargc;
char **myargv;

//
// M_CheckParm
// Checks for the given parameter
// in the program's command line arguments.
// Returns the argument number (1 to argc-1)
// or 0 if not present
int M_CheckParm(char *check)
{
    for (int i = 1; i < myargc; i++)
    {
        if (!strcasecmp(check, myargv[i]))
            return i;
    }

    return 0;
}
