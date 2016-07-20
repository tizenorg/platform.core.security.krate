CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(org.tizen.krate-setup-wizard C)

INCLUDE_DIRECTORIES(./include)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(SETUP_WIZARD REQUIRED
	notification
	zone
)

INCLUDE_DIRECTORIES(${EFL_APP_INCLUDE_DIRS} ${SETUP_WIZARD_INCLUDE_DIRS})
LINK_DIRECTORIES(${EFL_APP_LIBRARY_DIRS} ${SETUP_WIZARD_LIBRARY_DIRS})

SET(PACKAGE_SRC src/main.cpp
		src/setup-wizard.cpp
#		src/welcome.cpp
		src/common/window.cpp
		src/common/application.cpp
		src/common/layout.cpp
		src/common/naviframe.cpp
		src/common/conformant.cpp
		src/common/shell.cpp
		src/common/widget.cpp
		src/common/entry.cpp
		src/common/popup.cpp
		src/common/progressbar.cpp
		src/common/timer.cpp
		src/common/genlist.cpp
		src/widget.cpp
		)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

ADD_EXECUTABLE(${PROJECT_NAME} ${PACKAGE_SRC})
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${APP_INSTALL_PREFIX}/${PROJECT_NAME}/bin)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${EFL_APP_LIBRARIES} ${SETUP_WIZARD_LIBRARIES})

ADD_CUSTOM_TARGET(${PROJECT_NAME}.edj
        COMMAND edje_cc -no-save -id ${CMAKE_CURRENT_SOURCE_DIR}/res/images
        ${CMAKE_CURRENT_SOURCE_DIR}/res/edje/${PROJECT_NAME}.edc
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.edj
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/edje/${PROJECT_NAME}.edc)

ADD_DEPENDENCIES(${PROJECT_NAME} ${PROJECT_NAME}.edj)
INSTALL(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.edj DESTINATION ${APP_INSTALL_PREFIX}/${PROJECT_NAME}/res)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/KrateManifest.xml DESTINATION ${APP_INSTALL_PREFIX}/${PROJECT_NAME}/res/data)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.xml DESTINATION ${APP_SHARE_PACKAGES_DIR})
