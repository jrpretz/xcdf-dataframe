# bad build script.
# need to get it into cmake or something. It needs python, xcdf

g++ `hawc-config --cppflags` -pthread -fno-strict-aliasing -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -fPIC -I/home/pretz/HAWC/software/ApeInstalled-new/External/python/2.7.6/lib/python2.7/site-packages/numpy/core/include -I/home/pretz/HAWC/software/ApeInstalled-new/External/python/2.7.6/include/python2.7 -c read_xcdf.c -o read_xcdf.o


gcc `hawc-config --ldflags` -pthread -shared read_xcdf.o -L/home/pretz/HAWC/software/ApeInstalled-new/External/python/2.7.6/lib -lpython2.7 -o read_xcdf.so
