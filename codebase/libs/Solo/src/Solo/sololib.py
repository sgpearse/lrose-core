""" Python wrapper for the C shared library mylib"""
import sys, platform
import ctypes, ctypes.util

# Find the library and load it
#sololib_path = ctypes.util.find_library("/tmp/LibSolo.so")
#if not sololib_path:
#    print("Unable to find the specified library.")
#    sys.exit()

try:
    # sololib = ctypes.CDLL(sololib_path)
    sololib = ctypes.CDLL("/tmp/LibSolo.so")
except OSError:
    print("Unable to load the system C library")
    sys.exit()

SampleAddInt = sololib.SampleAddInt
SampleAddInt.argtypes = [ctypes.c_int, ctypes.c_int]
SampleAddInt.restype = ctypes.c_int

despeckle = sololib.se_despeckle
despeckle.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_size_t, ctypes.c_float, ctypes.c_int, \
	ctypes.c_int, ctypes.POINTER(ctypes.c_bool)]
despeckle.restype = None
