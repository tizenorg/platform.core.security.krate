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

#ifndef __KLAY_UX_RADIO_H__
#define __KLAY_UX_RADIO_H__

#include "widget.h"

namespace klay {

class Radio : public Widget {
public:
	Radio(Widget *parent, Widget* group, int index)
	{
		nativeObject = elm_radio_add(Widget::object(parent));
		elm_radio_state_value_set(nativeObject, index);
		if (group != NULL)
			elm_radio_group_add(nativeObject, Widget::object(group));
	}
	virtual ~Radio() {}
};

} // namespace klay

#endif //!__KLAY_UX_RADIO_H__
