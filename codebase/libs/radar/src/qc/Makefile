###########################################################################
#
# Mike Dixon, RAP, NCAR, Boulder, Co, USA, 80307
#
# Sept 2016
#
###########################################################################

include $(RAP_MAKE_INC_DIR)/rap_make_macros

LOC_INCLUDES = -I../include -I../include/radar $(NETCDF4_INCS)
LOC_CFLAGS =

TARGET_FILE = ../libradar.a

#
# file lists
#

HDRS = \
	../include/radar/HcrVelFirFilt.hh \
	../include/radar/HcrSurfaceVel.hh \
	../include/radar/IntfLocator.hh \
	../include/radar/SeaClutter.hh

CPPC_SRCS = \
	HcrVelFirFilt.cc \
	HcrSurfaceVel.cc \
	IntfLocator.cc \
	SeaClutter.cc

#
# general targets
#

include $(RAP_MAKE_INC_DIR)/rap_make_lib_module_targets

#
# local targets
#

depend: depend_generic

# DO NOT DELETE THIS LINE -- make depend depends on it.
