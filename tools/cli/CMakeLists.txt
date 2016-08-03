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

FILE(GLOB CLI_SRCS	krate-admin-cli.cpp
					${KRATE_MODULE}/session.cpp
					${KRATE_MODULE}/krate-builder.cpp
)

SET(CLI_NAME ${PROJECT_NAME}-admin-cli)

ADD_EXECUTABLE(${CLI_NAME} ${CLI_SRCS})
SET_TARGET_PROPERTIES(${CLI_NAME} PROPERTIES PREFIX ""
	COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_DIR}/krate"
	COMPILE_FLAGS "-fPIE"
	LINK_FLAGS "-pie"
)

PKG_CHECK_MODULES(CLI_DEPS	REQUIRED
							klay
							glib-2.0
							libxml-2.0
							capi-appfw-package-manager
							capi-appfw-app-manager
)

INCLUDE_DIRECTORIES(SYSTEM ${CLI_DEPS_INCLUDE_DIRS} ${KRATE_MODULE} ${KRATE_LIB})
TARGET_LINK_LIBRARIES(${CLI_NAME} ${CLI_DEPS_LIBRARIES} krate)

INSTALL(TARGETS ${CLI_NAME} DESTINATION sbin)
