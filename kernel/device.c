/*
 * File      : device.c
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

#include <var_define.h>

DEVICE device_queue_head;

void device_queue_init(void)
{
	list_init(&device_queue_head.list);
}

int device_register(DEVICE *device)
{
	if (device == NULL) {
		OS_LOG("Device null\n");
		return NO_DEVICE;
	}

	list_insert_behind(&device_queue_head.list, &device->list);

	return 0;
}

int device_unregister(DEVICE *device)
{
	if (device == NULL) {
		OS_LOG("Device null\n");
		return NO_DEVICE;
	}

	list_delete(&device->list);

	return 0;
}

int ops_init(OPERATIONS *ops, FUNC write, FUNC read)
{
	if (ops == NULL)
		OS_LOG("OPS null\n");

	ops->write = write;
	ops->read  = read;

	return 0;
}

int device_init(DEVICE *device, const U8 *name, OPERATIONS *ops)
{
	if ((device == NULL) || (ops == NULL)) {
		OS_LOG("Something error\n");
	}

	device->name = name;
	device->ops  = ops;

	return 0;
}

int device_read(DEVICE *device, U8 *buff, U8 size)
{
	if (device == NULL) {
		OS_LOG("Not device\n");
		return NO_DEVICE;
	}

	if (device->ops->read)
		device->ops->read(buff, size);
	else
		OS_LOG("Device read null\n");

	return 0;
}

int device_write(DEVICE *device, U8 *buff, U8 size)
{
	if (device == NULL) {
		OS_LOG("Not device\n");
		return NO_DEVICE;
	}

	if (device->ops->write)
		device->ops->write(buff, size);
	else
		OS_LOG("Device write null\n");

	return 0;
}
