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
	Entry(Widget *parent)
	{
		nativeObject = elm_entry_add(Widget::object(parent));
		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(nativeObject);
	}

	virtual ~Entry() {}

	void setSingleLine(bool enable)
	{
		elm_entry_single_line_set(nativeObject, enable);
	}

	void setPassword(bool enable)
	{
		elm_entry_password_set(nativeObject, enable);
	}

	void setLimitFilter(int max_char_count, int max_byte_count)
	{
		Elm_Entry_Filter_Limit_Size limit_size = {
			.max_char_count = max_char_count,
			.max_byte_count = max_byte_count
		};

		elm_entry_markup_filter_append(nativeObject, elm_entry_filter_limit_size, &limit_size);
	}

	void set(const std::string& text)
	{
		elm_entry_entry_set(nativeObject, text.c_str());
	}

	std::string get() const
	{
		return elm_entry_entry_get(nativeObject);
	}

	bool isEmpty()
	{
		return elm_entry_is_empty(nativeObject);
	}
};

} // namespace klay

#endif /*__KLAY_UX_ENTRY_H__*/
