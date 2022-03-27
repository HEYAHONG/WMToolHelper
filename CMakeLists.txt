

cmake_minimum_required(VERSION 3.8)

Set(PROJECT_NAME WMToolHelper)

Project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

option(STATIC_BUILD "static build" OFF)


if(WIN32)
	set(MAIN_LIBS "-static-libgcc -static-libstdc++")
endif()

if(STATIC_BUILD)
	set(MAIN_LIBS "${MAIN_LIBS} -static")
endif()


file(GLOB MAIN_CXX_C_FILES *.cpp *.c )
list(APPEND  MAIN_SRCS ${MAIN_CXX_C_FILES})

if(WIN32)
	file(GLOB_RECURSE MAIN_RC_FILES win32/*.rc)
	list(APPEND  MAIN_SRCS ${MAIN_RC_FILES})
endif()


add_executable(${PROJECT_NAME} ${MAIN_SRCS})
target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})

#添加线程库
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})

#添加wxWidgets库
if(STATIC_BUILD)
        set(wxWidgets_USE_STATIC ON)
endif()
find_package(wxWidgets REQUIRED all )
include(${wxWidgets_USE_FILE})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

#安装
INSTALL(TARGETS ${PROJECT_NAME}
       RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
       LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
       ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

include(GNUInstallDirs)

if(WIN32)
INSTALL(FILES win32/wm_tool.exe
       DESTINATION ${CMAKE_INSTALL_BINDIR}
)
ELSE()
#添加对wm_tool的编译
add_executable(wm_tool wm_tool/wm_tool.c)
target_link_libraries(wm_tool ${MAIN_LIBS} ${CMAKE_THREAD_LIBS_INIT}) 

#添加对wm_getvar的编译
add_executable(wm_getver wm_tool/wm_getver.c)
target_link_libraries(wm_getver ${MAIN_LIBS} ${CMAKE_THREAD_LIBS_INIT})

INSTALL(TARGETS wm_getver wm_tool
       RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
       LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
       ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
 
ENDIF()

