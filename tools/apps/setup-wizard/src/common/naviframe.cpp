#include "naviframe.h"

NaviItem::NaviItem(Elm_Object_Item* object) :
	item(object)
{
}

NaviFrame::NaviFrame(Widget* parent)
{
	nativeObject = elm_naviframe_add(Widget::object(parent));
	eext_object_event_callback_add(nativeObject, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
}

NaviFrame::~NaviFrame()
{
}

NaviItem NaviFrame::push(Widget* content)
{
	NaviItem item(elm_naviframe_item_push(nativeObject, NULL, NULL, NULL, Widget::object(content), NULL));
	item.setTitleEnabled(false, true);

	return item;
}

NaviItem NaviFrame::push(Widget* content, const std::string& title)
{
	NaviItem item(elm_naviframe_item_push(nativeObject, title.c_str(), NULL, NULL, Widget::object(content), NULL));
	item.setTitleEnabled(true, true);

	return item;
}

void NaviFrame::pop()
{
	elm_naviframe_item_pop(nativeObject);
}
