

cmake_minimum_required(VERSION 3.8)

#程序名称
Set(PROJECT_NAME WMToolHelper)
#主版本
set(PROJECT_MAJOR_VERSION_STR 1)
#次版本号
set(PROJECT_MINOR_VERSION_STR 0)
#修订号
set(PROJECT_REVISION_VERSION_STR 0)
#构建版本（以时间戳天数作为构建版本）
string(TIMESTAMP PROJECT_BUILD_TIME_SECOND "%s" UTC)
math(EXPR  PROJECT_BUILD_VERSION_STR "${PROJECT_BUILD_TIME_SECOND}/60/60/24"  OUTPUT_FORMAT DECIMAL)
#程序版本
set(PROJECT_VERSION_STR "${PROJECT_MAJOR_VERSION_STR}.${PROJECT_MINOR_VERSION_STR}.${PROJECT_REVISION_VERSION_STR}.${PROJECT_BUILD_VERSION_STR}")
message(STATUS "Version:${PROJECT_VERSION_STR}")
configure_file(Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Version.h)
if(WIN32)
configure_file(Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/win32/Version.h)
endif()

Project(${PROJECT_NAME} C CXX ASM)
Project(${PROJECT_NAME}	VERSION "${PROJECT_VERSION_STR}")

set(CMAKE_CXX_STANDARD 17)

option(STATIC_BUILD "static build" OFF)


if(WIN32)
	if(MSVC)
		add_compile_options(-utf-8 )
	else()
		set(MAIN_LIBS "-static-libgcc -static-libstdc++")
	endif()
endif()

if(STATIC_BUILD)
	if(NOT MSVC)
		set(MAIN_LIBS "${MAIN_LIBS} -static")
	endif()
endif()


file(GLOB MAIN_CXX_C_FILES *.cpp *.c )
list(APPEND  MAIN_SRCS ${MAIN_CXX_C_FILES})

if(WIN32)
	file(GLOB_RECURSE MAIN_RC_FILES win32/*.rc)
	list(APPEND  MAIN_SRCS ${MAIN_RC_FILES})
endif()

if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${MAIN_SRCS})
else()
	add_executable(${PROJECT_NAME} ${MAIN_SRCS})
endif()
target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})

#添加线程库
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})

#添加wxWidgets库
if(STATIC_BUILD)
        set(wxWidgets_USE_STATIC ON)
endif()
if(WIN32)
	find_package(wxWidgets REQUIRED core base adv aui webview html gl richtext )
else()
	find_package(wxWidgets REQUIRED all)
endif()
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
INSTALL(FILES win32/wm_tool.exe win32/putty.exe
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
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_STR}")
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

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

#打包NSIS安装包
message(STATUS "this is a windows system")
set(CPACK_PACKAGE_NAME  "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_STR}")
set(CPACK_PACKAGE_VENDOR "HEYAHONG")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "WMToolHelper By HYH")
set(CPACK_PACKAGE_DESCRIPTION "WMToolHelper By HYH")
set(CPACK_PACKAGE_CONTACT    "2229388563@qq.com")
#设置安装程序图标
set(CPACK_PACKAGE_ICON_PATH_STRING "${CMAKE_CURRENT_SOURCE_DIR}/win32/window_resources.ico")
string(REPLACE "/" "\\\\" CPACK_PACKAGE_ICON ${CPACK_PACKAGE_ICON_PATH_STRING})
set(CPACK_NSIS_MUI_ICON ${CPACK_PACKAGE_ICON})
set(CPACK_NSIS_MUI_UNIICON ${CPACK_PACKAGE_ICON})
#安装前卸载
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
#设置在控制面板的名称
set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
#设置需要创建快捷方式的程序
set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME};${PROJECT_NAME} )
#设置LICENSE
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/win32/nsis/LICENSE.txt)
#设置Welcome
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_SOURCE_DIR}/win32/nsis/WELCOME.txt)
#设置readme
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/win32/nsis/README.txt)
set(CPACK_GENERATOR "NSIS")
include(CPack)

endif()
