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

#ifndef __RUNTIME_DATA_TYPE_H__
#define __RUNTIME_DATA_TYPE_H__
#include <string>
#include <vector>
#include <utility>

#include "reflection.h"

struct Void {
	NO_REFLECTABLE_PROPERTY
};

struct String {
	String() = default;
	String(const String& str) : value(str.value) {}
	String(String&& str) : value(std::move(str.value)) {}

	String(const char* str) : value(str) {}
	String(const std::string& str) : value(str) {}

	String& operator=(const String& str)
	{
		if (this != &str) {
			value = str.value;
		}

		return *this;
	}

	String& operator=(String&& str)
	{
		if (this != &str) {
			value = std::move(str.value);
		}
		return *this;
	}

	std::string value;
	REFLECTABLE(value)
};

struct StringPair {
	std::string first;
	std::string second;
	REFLECTABLE(first, second)
};

struct Status {
	Status() : code(0) {}
	Status(int v) : code(v) {}

	operator int() const
	{
		return code;
	}

	int code;
	REFLECTABLE(code)
};

template<typename T>
struct Vector {
	std::vector<T> value;
	REFLECTABLE(value)
};

#endif //!__RUNTIME_DATA_TYPE_H__
