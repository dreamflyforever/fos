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
  * Shanjin Yang   2017-6-14         modify the open and close API, add handle of
 * 				    the many times device-init.
 */

#include <var_define.h>

DEVICE device_queue_head;

void device_queue_init(void)
{
	static int times;
	times++;
	if (times >= 2) {
		printf("device initial %d times\n", times);
		return ;
	}

	list_init(&device_queue_head.list);
}

int device_register(DEVICE * device, const char * name, OPERATIONS * ops)
{
	if (device == NULL) {
		OS_LOG("Device null\n");
		return NO_DEVICE;
	}

	device->name = name;
	device->ops = ops;
	device->flag = 0;
	device->open_count++;
	list_insert_behind(&device_queue_head.list, &device->list);

	return 0;
}

int device_unregister(DEVICE * device)
{
	if (device == NULL) {
		OS_LOG("Device null\n");
		return NO_DEVICE;
	}

	ops_init(device->ops, NULL, NULL, NULL, NULL, NULL);
	list_delete(&device->list);

	return 0;
}

int ops_init(OPERATIONS * ops,
	     int (*open)(char *arg, U8 flag),
	     FUNC write,
	     FUNC read,
	     int (*ioctrl)(U8 cmd, void *arg),
	     int (*close)()
)
{
	if (ops == NULL)
		OS_LOG("OPS null\n");

	ops->open = open;
	ops->ioctrl = ioctrl;
	ops->write = write;
	ops->read = read;
	ops->close = close;

	return 0;
}

int device_read(DEVICE * device, char * buff, U8 size)
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

int device_write(DEVICE * device, char * buff, U8 size)
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

int device_ioctrl(DEVICE * device, U8 cmd, void *arg)
{
	if (device == NULL) {
		OS_LOG("Not device\n");
		return NO_DEVICE;
	}

	if (device->ops->ioctrl)
		device->ops->ioctrl(cmd, arg);
	else
		OS_LOG("Device cmd null\n");

	return 0;
}

DEVICE *device_find(char * name)
{
	DEVICE *device;
	LIST *tmp = &device_queue_head.list;

	while (!is_list_last(tmp)) {

		device = list_entry(tmp->next, DEVICE, list);
		tmp = tmp->next;

		if (strcmp((const char *)device->name, (const char *)name) == 0) {
			return device;
		}
	}

	return NULL;
}

int device_open(DEVICE *device, char * name, U8 flag)
{
	if (!device) {
		OS_LOG("No device\n");
		return NO_DEVICE;
	}

	if (device->ops->open) {
		device->ops->open(name, flag);
	}

	device->flag |= flag;

	/*todo:open many times. XXX */
	device->open_count++;

	return 0;
}

int device_close(DEVICE *device)
{
	if (!device) {
		OS_LOG("No device\n");
		return NO_DEVICE;
	}

	if (device->ops->close) {
		device->ops->close();
	}

	device->flag = 0;
	device->open_count--;

	return 0;
}

int device_traverse()
{
	DEVICE *device;
	LIST *tmp = &device_queue_head.list;
	printf("device list :\n");
	while (!is_list_last(tmp)) {

		device = list_entry(tmp->next, DEVICE, list);
		tmp = tmp->next;

		if (device->name != NULL) {
			printf("%s\n", device->name);
		}
	}

	return 0;
}
