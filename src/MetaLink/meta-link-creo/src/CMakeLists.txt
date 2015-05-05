
cmake_minimum_required (VERSION 2.8)

project (META_LINK_CREO)

set (CMAKE_BUILD_TYPE Release)

set (CMAKE_MODULE_LINKER_FLAGS  "/machine:x64")
set (CMAKE_EXE_LINKER_FLAGS  "/machine:x64")
set (CMAKE_SHARED_LINKER_FLAGS  "/machine:x64")

set(META_LINK_CREO_SRCS 
 BridgeClient.cc     
 FramedEdit.cc       
 gen/AssemblyInterface.pb.cc  
 gen/MetaLinkMsg.pb.cc        
 gen/XsdTypes.pb.cc           )

set(META_LINK_CREO_EXEC_SRC 
	meta-link-main.cc )   

set(META_LINK_CREO_HEAD 
 BridgeClient.h     
 FramedEdit.h       
 gen/AssemblyInterface.pb.h  
 gen/MetaLinkMsg.pb.h        
 gen/XsdTypes.pb.h           )

set (THIRD_ROOT "../../../../3rdParty")
if (EXISTS "${THIRD_ROOT}")

  set (META_LINK_CREO_VERSION_MAJOR 1)
  set (META_LINK_CREO_VERSION_MINOR 0)
  
  include_directories("${PROJECT_SOURCE_DIR}")
  add_library (meta-link-lib ${META_LINK_CREO_SRCS} ${META_LINK_CREO_HEAD})
  
  link_directories (${META_LINK_CREO_BINARY_DIR})
  add_executable (meta-link-creo ${META_LINK_CREO_EXEC_SRC} )
  target_link_libraries (meta-link-creo meta-link-lib)
  
  set(Boost_USE_STATIC_LIBS        ON)
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_RUNTIME    OFF)
  
  set (Boost_FIND_REQUIRED TRUE) 

  set (BOOST_ROOT "${THIRD_ROOT}/boost-1_53_0") 
  set (BOOST_INCLUDEDIR "${BOOST_ROOT}")      
  set (BOOST_LIBRARYDIR "${BOOST_ROOT}/x64/lib")      
  set (Boost_NO_SYSTEM_PATHS ON) 

  FIND_PACKAGE(Boost COMPONENTS atomic system date_time regex thread chrono program_options)
  IF (Boost_FOUND)
  	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  	ADD_DEFINITIONS( "-DHAS_BOOST" )
  	TARGET_LINK_LIBRARIES(meta-link-creo ${Boost_LIBRARIES})
  ELSE()
  	INCLUDE_DIRECTORIES(${Boost_INCLUDEDIR})
  	ADD_DEFINITIONS( "-DHAS_BOOST" )
  	TARGET_LINK_LIBRARIES(meta-link-creo ${Boost_LIBRARIES})
  ENDIF()
  
  set(PROTOBUF_SRC_ROOT_FOLDER "${THIRD_ROOT}/protoc-2.5.0-win32")
  
  set (PROTOBUF_PROTOC_EXECUTABLE "${PROTOBUF_SRC_ROOT_FOLDER}/protoc25.exe")
  set (PROTOBUF_LIBRARY "${PROTOBUF_SRC_ROOT_FOLDER}/libprotobuf.lib")
  set (PROTOBUF_INCLUDE_DIR "${PROTOBUF_SRC_ROOT_FOLDER}")
  INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_PROTOBUF" )
  TARGET_LINK_LIBRARIES(meta-link-creo ${PROTOBUF_LIBRARIES})
  
  
  # Windows specific definitions
  IF (WIN32) 
  	ADD_DEFINITIONS( "-D_SCL_SECURE_NO_WARNINGS" )
  	ADD_DEFINITIONS( "-D_WIN32_WINNT=0x0501" ) # Windows XP target
  ENDIF()
  
else ()
  message ("could not find ${THIRD_ROOT}")	

endif ()

