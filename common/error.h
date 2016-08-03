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

#ifndef __RUNTIME_ERROR_H__
#define __RUNTIME_ERROR_H__

#include <string>

namespace runtime {

class Error {
public:
	static std::string message();
	static std::string message(int errorCode);
	static int lastErrorCode();
};

std::string GetSystemErrorMessage();
std::string GetSystemErrorMessage(int errorCode);

} // namespace runtime
#endif //__RUNTIME_ERROR_H__
