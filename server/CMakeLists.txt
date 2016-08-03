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

SET(TARGET ${PROJECT_NAME})

SET(FOUNDATION		main.cpp
					server.cpp
					app-bundle.cpp
					launchpad.cpp
					packman.cpp
					zone.cpp
					app-proxy.cpp
					package-proxy.cpp
)

SET(DEPENDENCY		glib-2.0
					gio-2.0
					bundle
					aul
					appsvc
					pkgmgr
					pkgmgr-info
					libtzplatform-config
					capi-system-system-settings
					notification
					libgum
					cynara-client
					cynara-session
					auth-fw-admin
)

ADD_EXECUTABLE(${TARGET} ${FOUNDATION})

PKG_CHECK_MODULES(SERVER_DEPS REQUIRED ${DEPENDENCY})

INCLUDE_DIRECTORIES(SYSTEM ${SERVER_DEPS_INCLUDE_DIRS} ${KRATE_COMMON} ${KRATE_SERVER})

TARGET_LINK_LIBRARIES(${TARGET} ${SERVER_DEPS_LIBRARIES} pthread krate-common)

SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "-fPIE")
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "-pie")

TARGET_COMPILE_DEFINITIONS(${TARGET} PRIVATE
	CONF_PATH="${CONF_INSTALL_DIR}"
	DATA_PATH="${DATA_INSTALL_DIR}"
	RUN_PATH="${RUN_INSTALL_DIR}"
	DB_PATH="${DB_INSTALL_DIR}"
)

CONFIGURE_FILE(systemd/krate.service.in systemd/krate.service)

INSTALL(TARGETS ${TARGET} DESTINATION bin)
INSTALL(FILES systemd/krate.service DESTINATION ${SYSTEMD_UNIT_INSTALL_DIR})
