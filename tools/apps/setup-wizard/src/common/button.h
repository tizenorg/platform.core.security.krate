#ifndef __KLAY_BUTTON_H__
#define __KLAY_BUTTON_H__

#include "widget.h"

class Button : public Widget
{
public:
	Button(Widget* parent, const std::string& text)
	{
		nativeObject = elm_button_add(Widget::object(parent));
		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		setText(text);
		setStyle("bottom");
	}

	virtual ~Button()
	{
	}

	void addClickHandler(SignalHandler&& handler) {
		addSignalHandler("mouse,clicked,1", "event", std::move(handler));
	}
};

#endif /*__KLAY_BUTTON_H__ */
