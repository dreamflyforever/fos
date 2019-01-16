1. 解包 
./rbin -x -f   res.bin   -d  res 
./rbin -x -f   wakeup_common_mtmt010_20171016.bin   -d  wakeup_mtmt
资源中有个 wakeup.cfg.r， 打开有expwords和thresh_array，一个词对应一个阈值，把相应的阈值和唤醒词去掉就好
2. 打包
./rbin -z -d  res -f res.bin
./rbin -z -d  res -f wakeup_common_mtmt010_20171016.bin

