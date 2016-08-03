/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __RUNTIME_GMAINLOOP_ADAPTOR_H__
#define __RUNTIME_GMAINLOOP_ADAPTOR_H__

#include <gio/gio.h>

#include <string>

#include "mainloop.h"

namespace klay {

class GMainloopAdaptor {
public:
	GMainloopAdaptor(const Mainloop& mainloop);
	~GMainloopAdaptor();

	void run();
	void stop();

private:
	GIOChannel* channel;
	guint watchId;
};

} // namespace klay

#endif // !__RUNTIME_GMAINLOOP_ADAPTOR_H__
