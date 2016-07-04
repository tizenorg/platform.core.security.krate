#
# Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
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

PROJECT(zone-provider)

FILE(GLOB ZONE_PAM_SRCS		 zone-builder.cpp
							 session.cpp
							 zone.cpp
)

SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

SET(ZONE_PAM_NAME "pam_zone")
ADD_LIBRARY(${ZONE_PAM_NAME} MODULE ${ZONE_PAM_SRCS})
SET_TARGET_PROPERTIES(${ZONE_PAM_NAME} PROPERTIES  PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/zone"
    COMPILE_FLAGS "-fvisibility=hidden"
)

FIND_PATH(PAM_INCLUDE_DIR NAMES security/pam_appl.h security/pam_ext.h security/pam_modules.h
    HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES include)
FIND_LIBRARY(PAM_LIBRARY pam  HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES ${LIB_INSTALL_DIR})
MARK_AS_ADVANCED(PAM_INCLUDE_DIR PAM_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PAM DEFAULT_MSG PAM_LIBRARY PAM_INCLUDE_DIR)

INCLUDE_DIRECTORIES(${PAM_INCLUDE_DIR} ${DPM_COMMON})
TARGET_LINK_LIBRARIES(${ZONE_PAM_NAME} ${PAM_LIBRARY} dpm-common pthread)

TARGET_COMPILE_DEFINITIONS(${ZONE_PAM_NAME} PRIVATE
    CONF_PATH="${CONF_INSTALL_DIR}"
)

INSTALL(TARGETS ${ZONE_PAM_NAME} DESTINATION ${LIB_INSTALL_DIR}/security)
INSTALL(FILES data/DefaultZoneManifest.xml DESTINATION ${CONF_INSTALL_DIR}/zone RENAME owner.xml)
INSTALL(FILES data/zone_indicator_icon.png DESTINATION /opt/data/dpm)
INSTALL(FILES data/zone_noti_list_sub_icon.png DESTINATION /opt/data/dpm)
INSTALL(FILES pam.d/systemd-user-zone DESTINATION ${PAMD_INSTALL_DIR})
