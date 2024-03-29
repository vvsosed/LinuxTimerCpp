set( STORAGE_SRC
     tmrwrap.cpp )

execute_process(
  COMMAND uname -r
  OUTPUT_VARIABLE KERNEL_RELEASE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Find the headers
find_path(KERNELHEADERS_DIR
  include/linux/user.h
  PATHS /usr/src/linux-headers-${KERNEL_RELEASE}
)

message(STATUS "Kernel release: ${KERNEL_RELEASE}")
message(STATUS "Kernel headers: ${KERNELHEADERS_DIR}")

if (KERNELHEADERS_DIR)
  set(KERNELHEADERS_INCLUDE_DIRS
    ${KERNELHEADERS_DIR}/include
    ${KERNELHEADERS_DIR}/arch/x86/include
    CACHE PATH "Kernel headers include dirs"
  )
  set(KERNELHEADERS_FOUND 1 CACHE STRING "Set to 1 if kernel headers were found")
else (KERNELHEADERS_DIR)
  set(KERNELHEADERS_FOUND 0 CACHE STRING "Set to 1 if kernel headers were found")
endif (KERNELHEADERS_DIR)

#set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

message(STATUS "Kernel headers include dir: ${KERNELHEADERS_INCLUDE_DIRS}")
#include_directories( ${KERNELHEADERS_INCLUDE_DIRS} )

#link_directories(
#    /usr/local/lib
#)

add_library( tmrwrap STATIC ${STORAGE_SRC})

target_include_directories( tmrwrap PUBLIC \. )

target_link_libraries( tmrwrap rt )
