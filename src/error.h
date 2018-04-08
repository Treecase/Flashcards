/*
 * Errors/logging
 *
 */

#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>



#define fatal(...)  {dprintf (DEBUG, "Fatal: %s - ", __func__); \
                     dprintf (DEBUG, __VA_ARGS__);              \
                     exit (1);}

#define error(...)  {dprintf (DEBUG, "Error: %s - ", __func__); \
                     dprintf (DEBUG, __VA_ARGS__);}

#define log(...)    {if (GLOBAL_loglevel >= LOG_DEBUG) {        \
                     dprintf (DEBUG, "Log: %s - ", __func__);   \
                     dprintf (DEBUG, __VA_ARGS__);}}


extern int  DEBUG;
extern char GLOBAL_exit, GLOBAL_loglevel;

enum LOG_LEVEL_e {
    LOG_SILENT,
    /*LOG_NORMAL,*/
    LOG_DEBUG
};

#endif

