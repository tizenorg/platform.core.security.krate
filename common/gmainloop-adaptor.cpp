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

#include "gmainloop-adaptor.h"

#include "exception.h"

namespace klay {

namespace {

gboolean MainloopIteratorCallback(GIOChannel* channel, GIOCondition condition, gpointer data)
{
	Mainloop* mainloop = reinterpret_cast<Mainloop *>(data);
	mainloop->dispatch(-1);
	return TRUE;
}

void MainloopDestroyCallback(gpointer data)
{
}

} // namespace

GMainloopAdaptor::GMainloopAdaptor()
{
}

GMainloopAdaptor::attach(const int pollfd)
{
	channel = g_io_channel_unix_new(pollfd);
	if (channel == NULL) {
		throw Exception("Failed to create GIO channel");
	}

	watchId = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, G_IO_IN,
								  MainloopIteratorCallback,
								  &mainloop,
								  MainloopDestroyCallback);
}

GMainloopAdaptor::~GMainloopAdaptor()
{
	g_source_remove(watchId);
	g_io_channel_unref(channel);
}

void GMainloopAdaptor::iterate()
{
	g_main_context_iteration(NULL, TRUE);
}

void GMainloopAdaptor::stop()
{
}

} // namespace klay
