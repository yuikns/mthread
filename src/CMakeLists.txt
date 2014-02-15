# src/

# include settings
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/lib)
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/lib/include)
INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include)

# lib settings
LINK_DIRECTORIES(${${CMAKE_PROJECT_NAME}_BINARY_DIR}/lib)


# all related files
AUX_SOURCE_DIRECTORY(. DIR_TARGET)
ADD_LIBRARY (relfiles ${DIR_TARGET})


ADD_EXECUTABLE(mthread main.cc)

#  executable dependencies
ADD_DEPENDENCIES(mthread relfiles)

#  executable target links
TARGET_LINK_LIBRARIES(mthread relfiles)
TARGET_LINK_LIBRARIES(mthread pthread)

#  executable properties
SET_TARGET_PROPERTIES(mthread PROPERTIES OUTPUT_NAME "mthread")





