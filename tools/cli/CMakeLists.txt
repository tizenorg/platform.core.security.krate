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

PROJECT(krate-admin-cli)

FILE(GLOB KRATE_ADMIN_CLI_SRCS  krate-admin-cli.cpp
                                ${KRATE_MODULE}/session.cpp
                                ${KRATE_MODULE}/zone-builder.cpp
)

SET(KRATE_ADMIN_CLI_NAME "zone-admin-cli")

ADD_EXECUTABLE(${ZONE_CLI_NAME} ${ZONE_CLI_SRCS})
SET_TARGET_PROPERTIES(${ZONE_CLI_NAME} PROPERTIES PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/zone"
    COMPILE_FLAGS "-fPIE"
    LINK_FLAGS "-pie"
)

PKG_CHECK_MODULES(ZONE_CLI_DEPS  REQUIRED
                                 glib-2.0
                                 capi-appfw-package-manager
                                 capi-appfw-app-manager
)

INCLUDE_DIRECTORIES(SYSTEM ${ZONE_CLI_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${ZONE_MODULE} ${ZONE_LIBS})
TARGET_LINK_LIBRARIES(${ZONE_CLI_NAME} ${ZONE_CLI_DEPS_LIBRARIES} dpm-common zone)

INSTALL(TARGETS ${ZONE_CLI_NAME} DESTINATION sbin)
