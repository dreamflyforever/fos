import config

env = Environment(tools = ['mingw'],
	AS = config.AS, ASFLAGS = config.AFLAGS,
	CC = config.CC, CCFLAGS = config.CFLAGS,
	AR = config.AR, ARFLAGS = '-rc',
	LINK = config.LINK, LINKFLAGS = config.LFLAGS,
	CPPPATH = ['kernel/include', 'cpu/or1ksim/include', 'libc/include'])

env.PrependENVPath('PATH', config.EXEC_PATH)

src = Glob('libc/*.c')
src = src + Glob('cpu/or1ksim/*.S')
src = src + Glob('kernel/*.c')
src = src + Glob('cpu/or1ksim/*.c')
src = src + Glob('app/*.c')
src = src + Glob('middleware/shell_parser/*.c')
env.Program('fos',src)
