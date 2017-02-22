# AIspeech SDK standard API

---------

这份文档主要描述标准化可能需要移植的API接口，具体视RTOS而看。

> * aiengine 用户使用接口
> * 系统层次相关的API
> * 硬件相关的API
> * 应用层依赖的库
> * 具体实施

[AIspeech SDK 源码地址](https://gitlab.spetechcular.com/shanjin.yang/linux_asr)

## aiengine 用户使用接口
```c
struct aiengine *aiengine_new(const char *cfg);
int aiengine_delete(struct aiengine *engine);
int aiengine_start(struct aiengine *engine,
			const char *param,
			aiengine_callback callback,
			const void *usrdata);

int aiengine_feed(struct aiengine *engine, const void *data, int size);
int aiengine_stop(struct aiengine *engine);
int aiengine_cancel(struct aiengine *engine);
int check_provision(struct aiengine *agn);
```

## 系统层次相关的API
`time`, `malloc`

## 硬件相关
`record`, `key`

## 应用层依赖的库
* 因为各个RTOS app层能实现的该库的方式多种多样，所以部分RTOS需要port这一部分
 * url encode
 * json
 * http
 * speex
 * player

## 具体实施
根据buluedroid把需要需要移植的分成OSI， HAL 层。系统相关的函数和硬件相关的函数直接
函数飞出来，然后里面空是实现。
