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

#include <unistd.h>
#include <assert.h>
#include <sys/epoll.h>

#include <cstring>
#include <iostream>

#include <gio/gio.h>

#include "error.h"
#include "exception.h"
#include "mainloop.h"

#define MAX_EPOLL_EVENTS	16

namespace runtime {

namespace {

gboolean GIOCallback(GIOChannel* channel, GIOCondition condition, void *data)
{
	Mainloop* mainloop = reinterpret_cast<Mainloop*>(data);
	mainloop->dispatch(-1);
	return TRUE;
}

} // namespace

Mainloop::Mainloop() :
	pollFd(::epoll_create1(EPOLL_CLOEXEC)),
	stopped(false)
{
	if (pollFd == -1) {
		throw Exception(GetSystemErrorMessage());
	}
}

Mainloop::~Mainloop()
{
	if (!callbacks.empty()) {
		//assert(0 && "callback list is not empty");
	}

	::close(pollFd);
}

void Mainloop::addEventSource(const int fd, const Event events, Callback&& callback)
{
	epoll_event event;
	std::lock_guard<Mutex> lock(mutex);

	if (callbacks.find(fd) != callbacks.end()) {
		throw Exception("Event source already registered");
	}

	::memset(&event, 0, sizeof(epoll_event));

	event.events = events;
	event.data.fd = fd;

	if (::epoll_ctl(pollFd, EPOLL_CTL_ADD, fd, &event) == -1) {
		throw Exception(GetSystemErrorMessage());
	}

	callbacks.insert({fd, std::make_shared<Callback>(std::move(callback))});
}

void Mainloop::removeEventSource(const int fd)
{
	std::lock_guard<Mutex> lock(mutex);

	auto iter = callbacks.find(fd);
	if (iter == callbacks.end()) {
		return;
	}

	callbacks.erase(iter);

	::epoll_ctl(pollFd, EPOLL_CTL_DEL, fd, NULL);
}

bool Mainloop::dispatch(const int timeout)
{
	int nfds;
	epoll_event event[MAX_EPOLL_EVENTS];

	do {
		nfds = ::epoll_wait(pollFd, event, MAX_EPOLL_EVENTS, timeout);
	} while ((nfds == -1) && (errno == EINTR));

	if (nfds < 0) {
		throw Exception(GetSystemErrorMessage());
	}

	for (int i = 0; i < nfds; i++) {
		std::lock_guard<Mutex> lock(mutex);

		auto iter = callbacks.find(event[i].data.fd);
		if (iter == callbacks.end()) {
			continue;
		}

		std::shared_ptr<Callback> callback(iter->second);
		try {
			if ((event[i].events & (EPOLLHUP | EPOLLRDHUP))) {
				event[i].events &= ~EPOLLIN;
			}

			(*callback)(event[i].data.fd, event[i].events);
		} catch (std::exception& e) {
			std::cout << "EXCEPTION ON MAINLOOP" << std::endl;
		}
	}

	return true;
}

void Mainloop::stop()
{
	wakeupSignal.send();
}

void Mainloop::prepare()
{
	auto wakeupMainloop = [this](int fd, Mainloop::Event event) {
		wakeupSignal.receive();
		removeEventSource(wakeupSignal.getFd());
		stopped = true;
	};

	addEventSource(wakeupSignal.getFd(), EPOLLIN, wakeupMainloop);
}

void Mainloop::run(bool useGMainloop)
{
	prepare();

	if (useGMainloop) {
		GIOChannel* channel;
		channel = g_io_channel_unix_new(pollFd);
		if (channel == NULL) {
			std::cout << "GMAINLOOP CHANNEL ALLOC FAILED" << std::endl;
			return;
		}
		g_io_add_watch(channel, (GIOCondition)(G_IO_IN|G_IO_HUP), GIOCallback, this);
		g_io_channel_unref(channel);

		while (!stopped) {
			g_main_iteration(TRUE);
		}
	} else {
		while (!stopped) {
			dispatch(-1);
		}
	}
}

} // namespace runtime
