/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __ZONE_MANAGER__
#define __ZONE_MANAGER__

#include "data-type.h"

class ZoneManager {
public:
	enum State {
		Locked      = 0x01,
		Running     = 0x02,
	};

	ZoneManager();
	~ZoneManager();

	int createZone(const std::string& name, const std::string& manifest);
	int removeZone(const std::string& name);
	int lockZone(const std::string& name);
	int unlockZone(const std::string& name);

	int getZoneState(const std::string& name);

	std::vector<std::string> getZoneList(int state);

	int resetZonePassword(const std::string& name, const std::string& newPassword);
};

#endif // __ZONE_MANAGER__
