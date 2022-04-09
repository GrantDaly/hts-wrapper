# hts-wrapper

# get htslib as a submodule
git submodule update --init --recursive

compile htslib submodule
./configure --prefix=${CONDA_PREFIX} ...

