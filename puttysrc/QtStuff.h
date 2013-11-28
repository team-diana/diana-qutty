#ifndef QTSTUFF_H
#define QTSTUFF_H

#include <stdio.h>
#include <stdint.h>

#ifdef _WINDOWS
#include <windows.h>
#endif
#ifdef _LINUX
#include <sys/sysinfo.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define APPNAME "QuTTY"
#define QUTTY_RELEASE_VERSION "0.3-beta"


struct Filename {
    char path[FILENAME_MAX];
};

struct FontSpec {
    char name[64];
    int isbold;
    int height;
    int charset;
};

typedef uint32_t uint32;

typedef void* Context;

void init_ucs(Config *, struct unicode_data *);

/*
 * On Windows, copying to the clipboard terminates lines with CRLF.
 */
#define SEL_NL { 13, 10 }

#define DEFAULT_CODEPAGE CP_ACP

#define TICKSPERSEC 1000	       /* GetTickCount returns milliseconds */

#ifdef _WINDOWS
#define GETTICKCOUNT GetTickCount
#endif
#ifdef _LINUX
long GetTickCountLinux();
#define GETTICKCOUNT GetTickCountLinux
#endif

#define f_open(filename, mode, isprivate) ( fopen((filename).path, (mode)) )

/*
 * sk_getxdmdata() does not exist under Windows (not that I
 * couldn't write it if I wanted to, but I haven't bothered), so
 * it's a macro which always returns NULL. With any luck this will
 * cause the compiler to notice it can optimise away the
 * implementation of XDM-AUTHORIZATION-1 in x11fwd.c :-)
 */
#define sk_getxdmdata(socket, lenp) (NULL)


void qt_message_box(void * frontend, const char *title, const char *fmt, ...);
void qt_message_box_no_frontend(const char *title, const char *fmt, ...);

#define qt_critical_msgbox(frontend, fmt, ...) \
    qt_message_box(frontend, APPNAME " Fatal Error", fmt, __VA_ARGS__)

#define modalfatalbox(msg) \
    qt_message_box_no_frontend(APPNAME " Fatal Error", msg)

#define fatalbox(fmt, ...) \
    qt_message_box_no_frontend(APPNAME " Fatal Error", fmt, __VA_ARGS__)

#define qutty_assert(cond) do {\
    if(!(cond)) qt_message_box_no_frontend(APPNAME " Fatal Error", "fatal assert %s(%d)"#cond, __FUNCTION__, __LINE__); \
} while(0)

void qutty_connection_fatal(void *frontend, char *msg);

void notify_remote_exit(void *frontend);

#ifdef _WINDOWS
#define connection_fatal(frontend, fmt, ...) do { \
    char buf[1000]; \
    _snprintf(buf, sizeof(buf), fmt, __VA_ARGS__); \
    qutty_connection_fatal(frontend, buf); \
} while(0);
#endif
#ifdef _LINUX
#define connection_fatal(frontend, fmt, ...) do { \
    char buf[1000]; \
    sprintf(buf, fmt, __VA_ARGS__); \
    qutty_connection_fatal(frontend, buf); \
} while(0);
#endif

#ifdef __cplusplus
}
#endif
#endif // QTSTUFF_H
