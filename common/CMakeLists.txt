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

PROJECT(krate-common)

SET (COMMON_SOURCES     ${KRATE_COMMON}/error.cpp
						${KRATE_COMMON}/process.cpp
						${KRATE_COMMON}/eventfd.cpp
						${KRATE_COMMON}/mainloop.cpp
						${KRATE_COMMON}/filesystem.cpp
						${KRATE_COMMON}/thread-pool.cpp
						${KRATE_COMMON}/rmi/socket.cpp
						${KRATE_COMMON}/rmi/client.cpp
						${KRATE_COMMON}/rmi/message.cpp
						${KRATE_COMMON}/rmi/message-composer.cpp
						${KRATE_COMMON}/rmi/service.cpp
						${KRATE_COMMON}/rmi/connection.cpp
						${KRATE_COMMON}/rmi/notification.cpp
						${KRATE_COMMON}/audit/logger.cpp
						${KRATE_COMMON}/audit/null-sink.cpp
						${KRATE_COMMON}/audit/console-sink.cpp
						${KRATE_COMMON}/xml/node.cpp
						${KRATE_COMMON}/xml/parser.cpp
						${KRATE_COMMON}/xml/document.cpp
						${KRATE_COMMON}/xml/keepblanks.cpp
						${KRATE_COMMON}/db/column.cpp
						${KRATE_COMMON}/db/statement.cpp
						${KRATE_COMMON}/db/connection.cpp
						${KRATE_COMMON}/auth/user.cpp
						${KRATE_COMMON}/auth/group.cpp
						${KRATE_COMMON}/dbus/error.cpp
						${KRATE_COMMON}/dbus/variant.cpp
						${KRATE_COMMON}/dbus/connection.cpp
)

SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${COMMON_SOURCES})

PKG_CHECK_MODULES(COMMON_DEPS REQUIRED  libxml-2.0
										sqlite3
										gio-2.0
)

INCLUDE_DIRECTORIES(SYSTEM  ${KRATE_COMMON}
							${COMMON_DEPS_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${COMMON_DEPS_LIBRARIES})
