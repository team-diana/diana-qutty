#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "putty.h"

/*
 * Convenience functions to access the backends[] array
 * (which is only present in tools that manage settings).
 */

Backend *backend_from_name(const char *name)
{
    Backend **p;
    for (p = backends; *p != NULL; p++)
    if (!strcmp((*p)->name, name))
        return *p;
    return NULL;
}

Backend *backend_from_proto(int proto)
{
    Backend **p;
    for (p = backends; *p != NULL; p++)
    if ((*p)->protocol == proto)
        return *p;
    return NULL;
}
