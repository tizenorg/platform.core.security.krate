#ifndef __KLAY_BUTTON_H__
#define __KLAY_BUTTON_H__

#include "widget.h"

class Button : public Widget
{
public:
	Button(Widget* parent, const std::string& text);
	virtual ~Button();
};

Button::Button(Widget* parent, const std::string& text) : Widget(parent)
{
	nativeObject = elm_button_add(nativeParent);
	evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	elm_object_text_set(nativeObject, text.c_str());
//	elm_object_style_set(nativeObject, "bottom");
	setText(text);
	setStyle("bottom");
}

Button::~Button()
{
}

#endif /*__KLAY_BUTTON_H__ */
