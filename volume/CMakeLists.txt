#
# Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
SET(ZONE_VOLUME_MANAGER_TARGET	"zone-volume-manager")

SET(ZONE_VOLUME_MANAGER_SOURCES key-manager.cpp
								key-generator.cpp
								kernel-keyring.cpp
								main.cpp
)

PKG_CHECK_MODULES(ZONE_VOLUME_MANAGER_DEPS	REQUIRED
											klay
											key-manager
)

ADD_EXECUTABLE(${ZONE_VOLUME_MANAGER_TARGET} ${ZONE_VOLUME_MANAGER_SOURCES})

INCLUDE_DIRECTORIES(SYSTEM ${ZONE_VOLUME_MANAGER_DEPS_INCLUDE_DIRS} ${ZONE_LIBS})

TARGET_LINK_LIBRARIES(${ZONE_VOLUME_MANAGER_TARGET} ${ZONE_VOLUME_MANAGER_DEPS_LIBRARIES})

INSTALL(TARGETS ${ZONE_VOLUME_MANAGER_TARGET} DESTINATION sbin)
