cmake_minimum_required(VERSION 2.6)

project(LocalWeightingMex)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
#set(MATLAB_ROOT "${MATLAB_ROOT} /home/carlo/MATHWORKS")
find_package(MyMatlab)
include_directories(${MATLAB_INCLUDE_DIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=c99 -DMX_COMPAT_32 -DMATLAB_MEX_FILE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DMX_COMPAT_32 -DMATLAB_MEX_FILE")

include(FindOpenMP)
if(OPENMP_FOUND)
   	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)

# Include Eigen because it's needed by the localweighting functions
include_directories(eigen)

add_mex(localweighting_dense_symm_mx localweighting.cpp localweighting.h localweighting_dense_symm_mx.cpp)
add_mex(localweighting_dense_asymm_mx localweighting.cpp localweighting.h localweighting_dense_asymm_mx.cpp)
add_mex(localweighting_sparse_symm_mx localweighting.cpp localweighting.h localweighting_sparse_symm_mx.cpp)
add_mex(localweighting_sparse_asymm_mx localweighting.cpp localweighting.h localweighting_sparse_asymm_mx.cpp)

# Add a test executable 
add_executable(test_localweighting test_localweighting.cpp localweighting.cpp localweighting.h)

# Add the csv dense2sparse converter
add_executable(csv_dense2sparse csv_dense2sparse.cpp)
