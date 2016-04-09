#ifndef __SERVICELOG_H_INCLUDED__
#define __SERVICELOG_H_INCLUDED__

extern "C" {
    typedef void (*print_log)(const char *fmt, ...);
    extern void  printOutput(const char *fmt, ...);
}

#endif // __SERVICELOG_H_INCLUDED__
