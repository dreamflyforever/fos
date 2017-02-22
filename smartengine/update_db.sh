#!/bin/sh
find ./  -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.py" -o -name "*.s" > cscope.files
cscope -Rbq -i cscope.files
ctags -R --exclude=.git
