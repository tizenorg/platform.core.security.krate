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

#ifndef __RUNTIME_ARRAY_H__
#define __RUNTIME_ARRAY_H__

#include <vector>
#include <utility>

namespace runtime {

template <typename T>
class Array final {
public:
	Array() = delete;
	Array(std::vector<T> &&list) :
		list(std::move(list)), it(this->list.begin())
	{
	}

	Array(const std::vector<T> &list) :
		list(list), it(this->list.begin())
	{
	}

	T *next()
	{
		if (it != list.end()) {
			return &(*it++);
		}
		return NULL;
	}

	bool isEnd()
	{
		return it == list.end();
	}

private:
	std::vector<T> list;
	typename std::vector<T>::iterator it;
};

} // namespace runtime

#endif // __RUNTIME_ARRAY_H__
