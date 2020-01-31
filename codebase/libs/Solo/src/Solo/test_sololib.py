
import numpy as np
import sololib
import ctypes

#
test_name = "no clipping; no bad flags; no boundary"
#
data = np.array([3,4,5,6], dtype=float)
newData = np.array([0,0,0,0], dtype=float)
bnd = np.array([1,1,1,1], dtype=bool)
p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 5

nGates = len(data)
clip_gate = nGates
newData_expected = newData  # np.array([0,0,0,0], dtype=float)

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()


#
test_name = "clipping; no bad flags; no boundary"
#
data = np.array([3,4,5,6], dtype=float)
newData = np.array([0,0,0,0], dtype=float)
bnd = np.array([1,1,1,1], dtype=bool)
p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 5

nGates = len(data)
clip_gate = int(nGates/2)
newData_expected = data  # np.array([0,0,0,0], dtype=float)

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)
except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()



#
test_name = "no clipping; speckle; no boundary"
#
data = np.array([3,-3, 5, 5, 5, -3 ,6], dtype=float)
newData = np.array([0,0,0,0, 0, 0, 0], dtype=float)
bnd = np.array([1,1,1,1,1,1,1], dtype=bool)
p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 3

nGates = len(data)
clip_gate = nGates
newData_expected = np.array([3,-3, -3, -3, -3, -3 ,6], dtype=float)

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)

except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()


#
test_name = "clipping ends before second speckle; no boundary"
# should clear first speckle, but not second speckle because clip_gate ends 
# before second speckle
data =             np.array([3,-3, -3, 5, 5, 5, -3, 5, 5, -3], dtype=float)
newData =          np.array([0, 0,  0, 0, 0, 0,  0, 0, 0,  0], dtype=float)
newData_expected = np.array([3,-3, -3,-3,-3,-3, -3, 5, 5, -3], dtype=float)

bnd =              np.array([1, 1,  1, 1, 1, 1,  1, 1, 1,  1], dtype=bool)

p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 3

nGates = len(data)
clip_gate = 8

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)

except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()

#
test_name = "clipping; speckle; boundary ends before speckle"
# no data changed
data =             np.array([3,-3, -3, 5, 5, 5, -3, 5, 5, -3], dtype=float)
newData =          np.array([0, 0,  0, 0, 0, 0,  0, 0, 0,  0], dtype=float)
newData_expected = data # np.array([3,-3, -3,-3,-3,-3, -3, 5, 5, -3], dtype=float)

bnd =              np.array([1, 1,  0, 0, 0, 0,  0, 0, 0,  0], dtype=bool)

p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 3

nGates = len(data)
clip_gate = 8

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)

except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()



#
test_name = "clipping; speckle; boundary ends in middle of speckle"
#
# no data changed
data =             np.array([3,-3, -3, 5, 5, 5, -3, 5, 5, -3], dtype=float)
newData =          np.array([0, 0,  0, 0, 0, 0,  0, 0, 0,  0], dtype=float)
newData_expected = data # np.array([3,-3, -3,-3,-3,-3, -3, 5, 5, -3], dtype=float)

bnd =              np.array([1, 1,  1, 1, 0, 0,  0, 0, 0,  0], dtype=bool)

p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 3

nGates = len(data)
clip_gate = 8

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)

except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()

#
test_name = "clipping; speckle; boundary ends beyond speckle"
#
# 
data =             np.array([3,-3, -3, 5, 5, 5, -3, 5, 5, -3], dtype=float)
newData =          np.array([0, 0,  0, 0, 0, 0,  0, 0, 0,  0], dtype=float)
newData_expected = np.array([3,-3, -3, 5, 5, 5, -3,-3,-3, -3], dtype=float)

bnd =              np.array([0, 0,  0, 0, 0, 0,  1, 1, 1,  1], dtype=bool)

p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 3

nGates = len(data)
clip_gate = nGates

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)

except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()


#
test_name = "no clipping; speckle at beginning skip; zap speckle at end; no boundary"
#
data =             np.array([5, -3, -3, -3, 5, 5, 5], dtype=float)
newData =          np.array([0,  0,  0,  0, 0, 0, 0], dtype=float)
newData_expected = np.array([5, -3, -3, -3,-3,-3,-3], dtype=float)

bnd = np.array([1,1,1,1,1,1,1], dtype=bool)
p_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_newData = newData.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
p_bnd = bnd.ctypes.data_as(ctypes.POINTER(ctypes.c_bool))
bad_flag = -3
a_speckle = 3

nGates = len(data)
clip_gate = nGates

try:
	sololib.despeckle(p_data, p_newData, nGates, bad_flag, a_speckle, clip_gate, p_bnd)
	print(test_name, "=> ", np.array_equal(newData,  newData_expected))
	print(newData)
	print(newData_expected)

except OSError as err:
    print("OS error: {0}".format(err))
    sys.exit()
