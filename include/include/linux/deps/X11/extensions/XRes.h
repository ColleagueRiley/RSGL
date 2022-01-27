/*
   Copyright (c) 2002  XFree86 Inc
*/

#ifndef _XRES_H
#define _XRES_H

#include <X11/Xfuncproto.h>

/* v1.0 */

typedef struct {
  XID resource_base;
  XID resource_mask;
} XResClient;

typedef struct {
  Atom resource_type;
  unsigned int count;
} XResType;

/* v1.2 */

typedef enum {
  XRES_CLIENT_ID_XID,
  XRES_CLIENT_ID_PID,
  XRES_CLIENT_ID_NR
} XResClientIdType;

typedef enum {
  XRES_CLIENT_ID_XID_MASK = 1 << XRES_CLIENT_ID_XID,
  XRES_CLIENT_ID_PID_MASK = 1 << XRES_CLIENT_ID_PID
} XResClientIdMask;

typedef struct {
  XID           client;
  unsigned int  mask;
} XResClientIdSpec;

typedef struct {
  XResClientIdSpec spec;
  long          length;
  void         *value;
} XResClientIdValue;

typedef struct {
  XID           resource;
  Atom          type;
} XResResourceIdSpec;

typedef struct {
  XResResourceIdSpec spec;
  long          bytes;
  long          ref_count;
  long          use_count;
} XResResourceSizeSpec;

typedef struct {
  XResResourceSizeSpec  size;
  long                  num_cross_references;
  XResResourceSizeSpec *cross_references;
} XResResourceSizeValue;

_XFUNCPROTOBEGIN

/* v1.0 */

Bool XResQueryExtension (
   Display *dpy,
   int *event_base_return,
   int *error_base_return
);

Status XResQueryVersion (
   Display *dpy,
   int *major_version_return,
   int *minor_version_return
);

Status XResQueryClients (
   Display *dpy,
   int *num_clients,
   XResClient **clients
);

Status XResQueryClientResources (
   Display *dpy,
   XID xid,
   int *num_types,
   XResType **types
);

Status XResQueryClientPixmapBytes (
   Display *dpy,
   XID xid,
   unsigned long *bytes
);

/* v1.2 */

/* Returns an array of uint32_t values, not an array of long */
Status XResQueryClientIds (
   Display            *dpy,
   long                num_specs,
   XResClientIdSpec   *client_specs,   /* in */
   long               *num_ids,        /* out */
   XResClientIdValue **client_ids      /* out */
);

XResClientIdType XResGetClientIdType(XResClientIdValue* value);

/* return -1 if no pid associated to the value */
pid_t XResGetClientPid(XResClientIdValue* value);

void XResClientIdsDestroy (
   long                num_ids,
   XResClientIdValue  *client_ids
);

Status XResQueryResourceBytes (
   Display            *dpy,
   XID                 client,
   long                num_specs,
   XResResourceIdSpec *resource_specs, /* in */
   long               *num_sizes,      /* out */
   XResResourceSizeValue **sizes       /* out */
);

void XResResourceSizeValuesDestroy (
   long                num_sizes,
   XResResourceSizeValue *sizes
);

_XFUNCPROTOEND

#endif /* _XRES_H */
