SHELL=bash

src_dir=src/ \
	    third/ogg \
	    third/speex 
	    
src_c=${shell find $(src_dir) -name "*.c"}
c_objs=$(patsubst %.c,%.o,$(src_c))

lib_objs=$(c_objs)
objs=$(lib_objs)

############### compile section ###############
INC=-I . -I src/ -I third/ -I third/ogg -I third/speex

CFLAGS+=-Os -std=c99 -D _POSIX_C_SOURCE=200112L -D _BSD_SOURCE -fPIC  -Wall -Wno-variadic-macros -Wno-format-zero-length -D USE_SMALLFT -D FIXED_POINT -D SAVE_OGG $(INC) 
LFLAGS=

target=wav2ogg

%.o:%.c
		echo "compile ...";$(CC) $(CFLAGS) -c -o $@ $<; 

all: target

target: $(objs)
	$(CC) $(CFLAGS) ${WFLAGS} -c -o test/test_audioenc.o test/test_audioenc.c 
	ar cr libwav2ogg.a test/test_audioenc.o $(objs)

clean:
	-rm test/test_audioenc.o $(objs) $(target)
