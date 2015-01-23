BUILD = 'release'
PLATFORM 	= 'gcc'
#The position of gcc
EXEC_PATH 	= '/opt/orx/orxsys/staging_dir/toolchain-or1k/bin'
# toolchains
PREFIX = 'or1k-elf-'
CC = PREFIX + 'gcc'
AS = PREFIX + 'gcc'
AR = PREFIX + 'ar'
LINK = PREFIX + 'ld'
TARGET_EXT = 'elf'
SIZE = PREFIX + 'size'
OBJDUMP = PREFIX + 'objdump'
OBJCPY = PREFIX + 'objcopy'

DEVICE = ' -mhard-mul -mhard-div -msoft-float'
# DEVICE = ' -msoft-mul -msoft-div -msoft-float'

CFLAGS = DEVICE + ' -Werror -fno-builtin'
# AFLAGS = ' -c -x assembler-with-cpp' + DEVICE
AFLAGS = ' -c ' + DEVICE
# LFLAGS = DEVICE + ' -Bstatic -nostdlib -u _reset --stats -T link.lds'
LFLAGS = ' -Bstatic -nostdlib --stats -T link.lds -L /opt/or32-new/lib/gcc/or32-elf/4.5.1-or32-1.0rc4/libgcc.a'

#LFLAGS = ' -Bstatic -nostdlib --stats -T link.lds -L /opt/orx/orxsys/staging_dir/toolchain-or1k/lib/gcc/or1k-elf/4.9.0/compat-delay/soft-float/libgcc.a '

CPATH = ''
LPATH = ''

if BUILD == 'debug':
    CFLAGS += ' -O0 -g3 -Wall'
    AFLAGS += ' -g3'
else:
    CFLAGS += ' -O2 -g -Wall'

POST_ACTION = OBJDUMP + ' -x -d -S $TARGET >> system.map\n' \
    + OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'
    
