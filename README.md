FOS( Fast Operating System )
==========

It's an open soruce project about code RTOS(Real Time Operating Systems), named FOS, alias Chinese Operating System, not COPY OTHER SYSTEM, based on OpenRISC, and now it port to linux.

Now it's function will be complete step by step, I hope more and more people will be to make it better.

---------------------------------------------------------------------------

To build the FOS, tools need: scons and system gcc.  

CD to the RTOS directory and run:  
'scons'

This will produce execution files named 'fos'.  

To run the FOS for linux, run the shell script:  
'./start.sh'

---------------------------------------------------------------------------

SOURCE TREE:

 * kernel                   :      Core kernel sources
 * app                      :      User application sources
 * cpu                      :      CPU architecture ports
 * libc                     :      C function library.
 * middleware               :      The component for FOS
 * SConstruct and config.py :      Compile tools
 * start_or1ksim.sh         :      Run the system in the or1ksim

Contributions:
==============
This software was written by Shanjin Yang. License: GPL.  
Any question please contact me by <sjyangv0@qq.com>.
