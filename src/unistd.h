/*
 ******************** unistd.h **********************
 * This file is used to resolve the error resulted	*
 * from "#include <unistd.h>" in Lexer.cpp.			*
 * "unistd.h" is a UNIX header, so it's not present	*
 * in VC++. So, in this case we need an artificial	*
 * "unistd" header file.							*
 ****************************************************
 */

#if defined(_WIN32) || defined(_WIN64)
//Windows platform
#include <io.h>
#elif defined(__unix__)
//Unix platform
#include <unistd.h>
#endif