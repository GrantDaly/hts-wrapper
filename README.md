# for setting up htslib 




# downloade glibz source
./configure  --prefix=${CONDA_PREFIX}
make install

# Actually trying conda only, that is avoiding compiling gzip. Previous issue was in ./configure step
conda install zlib
 conda install -c anaconda bzip2 
 #conda install -c anaconda xz 
#  conda install -c rdonnelly xz-devel-amzn2-aarch64 

conda install zlib bzip2 xz libcurl

  
# get htslib as a submodule
git submodule update --init --recursive

# with gdb debug flags
CPPFLAGS="-ggdb -Wall -O2 -fvisibility=hidden -I${CONDA_PREFIX}/include" LDFLAGS="-L${CONDA_PREFIX}/lib" ./configure --prefix=${CONDA_PREFIX}
CPPFLAGS="-ggdb -Wall -O2 -fvisibility=hidden -I${CONDA_PREFIX}/include" LDFLAGS="-L${CONDA_PREFIX}/lib" ./configure --prefix=${CONDA_PREFIX}  --disable-lzma --enable-gcs


# most recently run. I'm not sure if rpath and Pkg-config are doing anything. At higher level I was adding rpath-link
CFLAGS="-ggdb -Wall -O2 -I${CONDA_PREFIX}/include -L${CONDA_PREFIX}/lib" LDFLAGS="-Wl,-rpath,${CONDA_PREFIX}/lib" PKG_CONFIG_PATH="${CONDA_PREFIX}/lib/pkgconfig/" ./configure --prefix=${CONDA_PREFIX}  --disable-lzma --enable-gcs


make && make install

# one test where I could compile against my installed htslib
https://stackoverflow.com/questions/6562403/i-dont-understand-wl-rpath-wl
# this doesn't work, with same linker error I've been seeing elsewhere.
g++ -I ../../envs/include/ -L../../envs/lib test.cpp -lhts

# including rpath works
g++ -I ../../envs/include/ -L../../envs/lib test.cpp -Wl,-rpath,../../envs/lib -lhts

# rpath with CONDA_PREFIX doesn't work
g++ -I ../../envs/include/ -L${CONDA_PREFIX}/lib -Wl,-rpath,${CONDA_PREFIX}/lib test.cpp -lhts
# rpath-link with CONDA_PREFIX does work
g++ -I ../../envs/include/ -L${CONDA_PREFIX}/lib -Wl,-rpath-link,${CONDA_PREFIX}/lib test.cpp -lhts


