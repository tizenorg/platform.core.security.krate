/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __KLAY_UX_ENTRY_H__
#define __KLAY_UX_ENTRY_H__

#include "widget.h"

namespace klay {

class Entry : public Widget {
public:
	enum Flags {
		SetSingleLine = (1 << 0),
		SetPassword = (1 << 1),
		SetReturnKeyDisabled = (1 << 2)
	};

	Entry(Widget *parent, int style);
	virtual ~Entry() {}

	void setLimitFilter(int max_char_count, int max_byte_count);
	void set(const std::string& text);
	std::string get() const;
	bool isEmpty();

	std::function<void()> onChanged;
	std::function<void()> onUnfocused;
};

} // namespace klay

#endif /*__KLAY_UX_ENTRY_H__*/
