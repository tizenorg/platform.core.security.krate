#include "naviframe.h"

Naviframe::Naviframe(Widget* parent) : Widget(parent)
{
	nativeObject = elm_naviframe_add(nativeParent);
	eext_object_event_callback_add(nativeObject, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
}

Naviframe::~Naviframe()
{
}

void Naviframe::push(Widget* frame)
{
	NaviItem *item = new NaviItem(this, frame);
	items.push(item);
}

void Naviframe::push(Widget* frame, const std::string& title)
{
	NaviItem *item = new NaviItem(this, frame);
	items.push(item);
}

void Naviframe::pop(void)
{
	//??
	elm_naviframe_item_pop(nativeObject);
	items.pop();
}
