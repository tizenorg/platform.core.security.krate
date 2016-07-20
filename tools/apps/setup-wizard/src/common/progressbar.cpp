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

#include "progressbar.h"

namespace klay {

Progressbar::Progressbar(Widget *parent, int style)
{
	nativeObject = elm_progressbar_add(Widget::object(parent));
	evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	if (style & SetPulse) {
		elm_progressbar_pulse_set(nativeObject, EINA_TRUE);
		elm_progressbar_pulse(nativeObject, EINA_TRUE);
	}

	if (style & SetHorizontal) {
		elm_progressbar_horizontal_set(nativeObject, EINA_TRUE);
	}
	evas_object_show(nativeObject);
}

Progressbar::~Progressbar()
{
}

void Progressbar::setPulseState(bool state)
{
	elm_progressbar_pulse(nativeObject, state);
}

double Progressbar::getValue()
{
	return elm_progressbar_value_get(nativeObject);
}

void Progressbar::setValue(double time)
{
	elm_progressbar_value_set(nativeObject, time);
}

} //namespace klay
