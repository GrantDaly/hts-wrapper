# hts-wrapper

# downloade glibz source
./configure  --prefix=${CONDA_PREFIX}
make install

# get htslib as a submodule
git submodule update --init --recursive

compile htslib submodule
./configure --prefix=${CONDA_PREFIX} ...

