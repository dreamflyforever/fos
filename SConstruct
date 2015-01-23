import glob
import config

env = Environment(tools = ['mingw'],
	AS = config.AS, ASFLAGS = config.AFLAGS,
	CC = config.CC, CCFLAGS = config.CFLAGS,
	AR = config.AR, ARFLAGS = '-rc',
	LINK = config.LINK, LINKFLAGS = config.LFLAGS,
	CPPPATH = ['kernel/include', 'cpu/include', 'libc/include'])#, 'middleware/uip/fos_uip', 'middleware/uip/uip', 'middleware/uip/apps/webserver'])
env.PrependENVPath('PATH', config.EXEC_PATH)

src = Glob('libc/*.c')
src = src + Glob('cpu/*.S')
src = src + Glob('kernel/*.c')
src = src + Glob('cpu/*.c')
src = src + Glob('app/*.c')
src = src + Glob('middleware/shell_parser/*.c')
#src = src + Glob('middleware/uip/fos_uip/*.c')
#src = src + Glob('middleware/uip/uip/*.c')
#src = src + Glob('middleware/uip/lib/*.c')
#src = src + Glob('middleware/uip/apps/webserver/*.c')
print src
env.Program('fos',src)

#obj = SConscript( 'kernel/SConscript', variant_dir='build/kernel', duplicate=0)
#obj = obj + SConscript('cpu/SConscript', variant_dir='build/cpu', duplicate=0)
#env.Program("fos",list(obj))  
