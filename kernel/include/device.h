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
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

typedef int (*FUNC) (U8 * buff, U8 size);

typedef struct OPERATIONS_STR {
	int (*open) (void *arg);
	int (*read) (U8 * buff, U8 size);
	int (*write) (U8 * buff, U8 size);
	int (*ioctrl) (U8 cmd, void *arg);
	int (*close) (void *arg);
} OPERATIONS;

typedef struct DEVICE_STR {
	LIST list;
	BOOL enable;
	const U8 *name;
	OPERATIONS *ops;
	U8 open_count;
	U8 flag;
} DEVICE;

extern DEVICE device_queue_head;

void device_queue_init(void);
int device_register(DEVICE * device, const U8 * name, OPERATIONS * ops);
int device_unregister(DEVICE * device);
int ops_init(OPERATIONS * ops,
	     int (*open) (void *arg),
	     FUNC write,
	     FUNC read,
	     int (*ioctrl) (U8 cmd, void *arg), int (*close) (void *arg)
    );
int device_open(U8 * name, U8 flag);
int device_read(DEVICE * device, U8 * buff, U8 size);
int device_write(DEVICE * device, U8 * buff, U8 size);
int device_ioctrl(DEVICE * device, U8 cmd, void *arg);
int device_close(U8 * name);

#endif
