LINK_DIRECTORIES(
	  ${VideoMan_BINARY_DIR}/bin/
	  ${CMAKE_CURRENT_SOURCE_DIR}/lib
)


INCLUDE_DIRECTORIES(
include
)

SET( YOURINPUTNAME_SRCS
	VMYOURINPUTNAME.cpp
	VMYOURINPUTNAMEInput.cpp
)
SET( YOURINPUTNAME_HEADERS
	VMYOURINPUTNAME.h
	VMYOURINPUTNAMEInput.h
)



IF (NOT MSVC60)
IF (NOT MSVC71)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
ENDIF (NOT MSVC71)
ENDIF (NOT MSVC60)

ADD_LIBRARY( VMYOURINPUTNAME SHARED
${VMYOURINPUTNAME_SRCS}
${VMYOURINPUTNAME_HEADERS}
)

TARGET_LINK_LIBRARIES( VMYOURINPUTNAME
	VideoMan
)

set_target_properties( VMYOURINPUTNAME PROPERTIES FOLDER "inputs")
add_dependencies( VMYOURINPUTNAME VideoMan )