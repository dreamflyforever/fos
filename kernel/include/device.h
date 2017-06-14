/*
 * File      : device.h
 * This file is part of FOS
 * 
 * Copyright (c) 2013, Shanjin Yang.<sjyangv0@gmail.com>
 * All rights reserved.
 *
 * The license and distribution terms for this file: GPL 
 *
 * Author         Date              content
 * Shanjin Yang   2015-3-30         the first version
 * Shanjin Yang   2017-6-14         modify the open and close API, add handle of
 * 				    the many times device-init.
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

typedef int (*FUNC) (char * buff, U8 size);

typedef struct OPERATIONS_STR {
	int (*open) (char *arg, U8 flag);
	int (*read) (char * buff, U8 size);
	int (*write) (char * buff, U8 size);
	int (*ioctrl) (U8 cmd, void *arg);
	int (*close) ();
} OPERATIONS;

typedef struct DEVICE_STR {
	LIST list;
	BOOL enable;
	const char *name;
	OPERATIONS *ops;
	U8 open_count;
	U8 flag;
} DEVICE;

extern DEVICE device_queue_head;

void device_queue_init(void);
int device_register(DEVICE * device, const char * name, OPERATIONS * ops);
int device_unregister(DEVICE * device);
int ops_init(OPERATIONS * ops,
	     int (*open)(char *arg, U8 flag),
	     FUNC write,
	     FUNC read,
	     int (*ioctrl)(U8 cmd, void *arg),
	     int (*close)()
    );
int device_open(DEVICE * device, char * name, U8 flag);
int device_read(DEVICE * device, char * buff, U8 size);
int device_write(DEVICE * device, char * buff, U8 size);
int device_ioctrl(DEVICE * device, U8 cmd, void *arg);
int device_close(DEVICE * device);

#endif
