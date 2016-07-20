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

#ifndef __KLAY_UX_PROGRESSBAR_H__
#define __KLAY_UX_PROGRESSBAR_H__

#include "widget.h"

namespace klay {

class Progressbar : public Widget {
public:
	enum Flags {
		SetPulse = (1 << 0),
		SetHorizontal = (1 << 2)
	};

	Progressbar(Widget *parent, int style);
	virtual ~Progressbar();

	void setPulseState(bool state);
	double getValue();
	void setValue(double time);
};

} //namespace klay

#endif
