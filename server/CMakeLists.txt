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

CONFIGURE_FILE(systemd/krate.service.in systemd/krate.service)

INSTALL(FILES systemd/krate.service DESTINATION ${SYSTEMD_UNIT_DIR})

INSTALL(FILES ${KRATE_TOOLS}/gumd-scripts/20_krate-add.post DESTINATION /etc/gumd/useradd.d)
INSTALL(FILES ${KRATE_TOOLS}/gumd-scripts/20_krate-remove.post DESTINATION /etc/gumd/userdel.d)

INSTALL(FILES ${KRATE_DATA}/DefaultManifest.xml DESTINATION ${CONF_DIR} RENAME owner.xml)
INSTALL(FILES ${KRATE_DATA}/indicator_icon.png DESTINATION ${APP_ICON_DIR_PREFIX}/krate)
INSTALL(FILES ${KRATE_DATA}/notification_sub_icon.png DESTINATION ${APP_ICON_DIR_PREFIX}/krate)
