env = Environment(CPPPATH = ['kernel/include', 'cpu/linux/include', 'libc/include', 'middleware/uip/uip', 'middleware/uip/fos', 'middleware/uip/apps/webserver'])
src = Glob('libc/*.c')
src = src + Glob('cpu/*.S')
src = src + Glob('kernel/*.c')
src = src + Glob('cpu/linux/*.c')
src = src + Glob('app/*.c')
src = src + Glob('middleware/shell_parser/*.c')
env.Library('foslib',src, CFLAGS = ['-g', '-fcolor-diagnostics'], CC = 'clang')
#env.SharedLibrary('foslib',src, CFLAGS = ['-g', '-fcolor-diagnostics', '-Werror'], CC = 'clang')
#for uip port
src = Glob('middleware/uip/apps/webserver/*.c')
src = src + Glob('middleware/uip/lib/*.c')
src = src + Glob('middleware/uip/uip/*.c')
src = src + Glob('middleware/uip/fos/*.c')
env.Program('fos',
		src,
		LIBS = 'foslib',
        	LIBPATH = ['.'], 
        	CCFLAGS = '-DHELLOSCONS',
		CFLAGS = ['-g', '-fcolor-diagnostics', '-fpack-struct', '-fno-strict-aliasing', '-Wno-unused-variable', '-Os', '-Wall'],
		CC = 'clang')
