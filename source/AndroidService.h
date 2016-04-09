#ifndef __ANDROIDSERVICE_H__
#define __ANDROIDSERVICE_H__


extern "C" {

typedef void (*output_log)(const char *fmt, ...);
    extern int startService();
    extern int stopService();
    extern int initService();
    extern void setPrintOutput(output_log output);
}
#endif // __ANDROIDSERVICE_H__
