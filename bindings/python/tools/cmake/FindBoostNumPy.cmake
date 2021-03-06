# - Try to find BoostNumPy

# BOOST_NUMPY_FOUND - System has BoostNumPy
# BOOST_NUMPY_INCLUDE_DIRS - The BoostNumPy include directories
# BOOST_NUMPY_LIBRARIES - The libraries needed to use BoostNumPy
# BOOST_NUMPY_DEFINITIONS - Compiler switches required for using BoostNumPy

SET (BOOST_NUMPY_ROOT_DIR ${BOOST_NUMPY_ROOT_DIR} "/usr/local")

FIND_PATH (BOOST_NUMPY_INCLUDE_DIR "boost/numpy.hpp" PATHS ${BOOST_NUMPY_ROOT_DIR} PATH_SUFFIXES "include" NO_DEFAULT_PATH)

FIND_LIBRARY (BOOST_NUMPY_LIBRARY NAMES "libboost_numpy.a" PATHS ${BOOST_NUMPY_ROOT_DIR} PATH_SUFFIXES "lib64" NO_DEFAULT_PATH)

SET (BOOST_NUMPY_INCLUDE_DIRS ${BOOST_NUMPY_INCLUDE_DIR})
SET (BOOST_NUMPY_LIBRARIES ${BOOST_NUMPY_LIBRARY})

# MESSAGE (STATUS "BOOST_NUMPY_ROOT_DIR = ${BOOST_NUMPY_ROOT_DIR}")
# MESSAGE (STATUS "BOOST_NUMPY_INCLUDE_DIRS = ${BOOST_NUMPY_INCLUDE_DIRS}")
# MESSAGE (STATUS "BOOST_NUMPY_LIBRARIES = ${BOOST_NUMPY_LIBRARIES}")

INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (BOOST_NUMPY DEFAULT_MSG BOOST_NUMPY_LIBRARY BOOST_NUMPY_INCLUDE_DIR)

MARK_AS_ADVANCED (BOOST_NUMPY_INCLUDE_DIR BOOST_NUMPY_LIBRARY)
