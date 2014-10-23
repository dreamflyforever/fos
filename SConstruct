import glob
import config

env = Environment(tools = ['mingw'],
	AS = config.AS, ASFLAGS = config.AFLAGS,
	CC = config.CC, CCFLAGS = config.CFLAGS,
	AR = config.AR, ARFLAGS = '-rc',
	LINK = config.LINK, LINKFLAGS = config.LFLAGS)
env.PrependENVPath('PATH', config.EXEC_PATH)

src = Glob('libc/*.c')
src = src + Glob('cpu/*.S')
src = src + Glob('kernel/*.c')
src = src + Glob('cpu/*.c')
src = src + Glob('app/*.c')
src = src + Glob('middleware/shell_parser/*.c')
print src
env.Program('hello',src)

#obj = SConscript( 'kernel/SConscript', variant_dir='build/kernel', duplicate=0)
#obj = obj + SConscript('cpu/SConscript', variant_dir='build/cpu', duplicate=0) 
#env.Program("hello",list(obj))  

