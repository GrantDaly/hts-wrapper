# hts-wrapper

# downloade glibz source
./configure  --prefix=${CONDA_PREFIX}
make install

# Actually trying conda only, that is avoiding compiling gzip. Previous issue was in ./configure step
conda install zlib
 conda install -c anaconda bzip2 
 conda install -c anaconda xz 
 
# get htslib as a submodule
git submodule update --init --recursive

compile htslib submodule
CPPFLAGS="-I${CONDA_PREFIX}/include" LDFLAGS="-L${CONDA_PREFIX}/lib" ./configure --prefix=${CONDA_PREFIX}

# with gdb debug flags
CPPFLAGS="-ggdb -Wall -O2 -fvisibility=hidden -I${CONDA_PREFIX}/include" LDFLAGS="-L${CONDA_PREFIX}/lib" ./configure --prefix=${CONDA_PREFIX}
make && make install
