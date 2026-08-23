#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    extern int myargc;
    extern char **myargv;

    // Returns the position of the given parameter
    // in the arg list (0 if not found).
    int M_CheckParm(char *check);

#ifdef __cplusplus
}
#endif