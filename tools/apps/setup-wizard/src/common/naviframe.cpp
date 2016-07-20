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

NaviItem::NaviItem(Widget* mainFrame, Widget *content) :
	WidgetItem(mainFrame)
{
	item = elm_naviframe_item_push(nativeParent->nativeObject, NULL, NULL, NULL, content->nativeObject, NULL);
	elm_naviframe_item_title_enabled_set(item, EINA_FALSE, EINA_TRUE);
}

NaviItem::NaviItem(Widget* mainFrame, Widget *content, const std::string& title) :
	WidgetItem(mainFrame)
{
	item = elm_naviframe_item_push(nativeParent->nativeObject, title.c_str(), NULL, NULL, content->nativeObject, NULL);
	elm_naviframe_item_title_enabled_set(item, EINA_TRUE, EINA_TRUE);
}

void NaviItem::addCallback(Callback&& callback)
{
	callbackInternal = std::move(callback);
	elm_naviframe_item_pop_cb_set(item, callbackDispatcher, this);
}
