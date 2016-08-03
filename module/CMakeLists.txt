#
# Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
# Licensed under the Apache License, Version 2.0(the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
FILE(GLOB PAM_SRCS	krate.cpp
					session.cpp
					krate-builder.cpp
)

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

SET(PAM_NAME pam_${PROJECT_NAME})

ADD_LIBRARY(${PAM_NAME} MODULE ${PAM_SRCS})
SET_TARGET_PROPERTIES(${PAM_NAME} PROPERTIES  PREFIX ""
	COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/krate"
	COMPILE_FLAGS "-fvisibility=hidden"
)

FIND_PATH(PAM_INCLUDE_DIR NAMES security/pam_appl.h security/pam_ext.h security/pam_modules.h
	HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES include)
FIND_LIBRARY(PAM_LIBRARY pam  HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES ${LIB_INSTALL_DIR})
MARK_AS_ADVANCED(PAM_INCLUDE_DIR PAM_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PAM DEFAULT_MSG PAM_LIBRARY PAM_INCLUDE_DIR)

PKG_CHECK_MODULES(PAM_DEPS	REQUIRED
							klay
							libxml-2.0
)

INCLUDE_DIRECTORIES(${PAM_INCLUDE_DIR} ${PAM_DEPS_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PAM_NAME} ${PAM_LIBRARY} ${PAM_DEPS_LIBRARIES} pthread)

TARGET_COMPILE_DEFINITIONS(${PAM_NAME} PRIVATE
	CONF_PATH="${CONF_DIR}"
)

CONFIGURE_FILE(pam.d/systemd-user.in pam.d/systemd-user-${PROJECT_NAME})

INSTALL(TARGETS ${PAM_NAME} DESTINATION ${LIB_INSTALL_DIR}/security)
INSTALL(FILES pam.d/systemd-user-${PROJECT_NAME} DESTINATION ${PAMD_DIR})
