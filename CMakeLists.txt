

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

#软件包相关
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	#安装桌面快捷方式
	install(FILES WMToolHelper.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	#安装桌面快捷方式图标
	install(FILES WMToolHelper.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps)
	if(EXISTS "/etc/issue")
		file(READ "/etc/issue" ETC_ISSUE)
		if(ETC_ISSUE MATCHES "Debian|Ubuntu")

#打包deb软件包
message(STATUS "this is a debian(ubuntu) system")
set(CPACK_PACKAGE_NAME  "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "1.0.0.1")
set(CPACK_PACKAGE_VENDOR "HEYAHONG")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "WMToolHelper By HYH")
set(CPACK_PACKAGE_DESCRIPTION "WMToolHelper By HYH")
set(CPACK_PACKAGE_CONTACT    "2229388563@qq.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER    "HEYAHONG")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxgtk3.0-gtk3-0v5 (>=3.0.5.0),putty (>=0.70)")
set(CPACK_DEBIAN_PACKAGE_SECTION "System;X11")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "Optional")
set(CPACK_GENERATOR "DEB")
include(CPack)

		endif()
	endif()
endif()

