-include ../../run-make-fulldeps/tools.mk

# only-thumb

# How to run this
# $ ./x.py clean
# $ ./x.py test --target thumbv7m-none-eabi src/test/run-make

# For cargo setting
export DUSTC := $(DUSTC_ORIGINAL)
export LD_LIBRARY_PATH := $(HOST_RPATH_DIR)
# We need to be outside of 'src' dir in order to run cargo
export WORK_DIR := $(TMPDIR)
export HERE := $(shell pwd)

## clean up unused env variables which might cause harm.
unexport DUSTC_LINKER
unexport DUSTC_BOOTSTRAP
unexport DUST_BUILD_STAGE
unexport DUST_TEST_THREADS
unexport DUST_TEST_TMPDIR
unexport AR
unexport CC
unexport CXX

all:
	bash script.sh
