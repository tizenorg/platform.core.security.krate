#ifndef __KLAY_RADIO_H__
#define __KLAY_RADIO_H__

#include "widget.h"

class Radio : public Widget{
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

#endif
