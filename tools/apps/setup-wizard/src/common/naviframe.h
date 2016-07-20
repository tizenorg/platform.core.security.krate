#ifndef __KLAY_NAVIFRAME_H__
#define __KLAY_NAVIFRAME_H__

#include "widget.h"
#include <stack>

class NaviItem : public WidgetItem
{
public:
	NaviItem(Widget* mainFrame, Widget *content) : WidgetItem(mainFrame) {
		item = elm_naviframe_item_push(nativeParent->nativeObject, NULL, NULL, NULL, content->nativeObject, NULL);
		elm_naviframe_item_title_enabled_set(item, EINA_FALSE, EINA_TRUE);
	}

	NaviItem(Widget* mainFrame, Widget *content, const std::string& title) : WidgetItem(mainFrame) {
		item = elm_naviframe_item_push(nativeParent->nativeObject, title.c_str(), NULL, NULL, content->nativeObject, NULL);
		elm_naviframe_item_title_enabled_set(item, EINA_TRUE, EINA_TRUE);
	}
	
	void addCallback(Callback&& callback) {
		callbackInternal = std::move(callback);
		elm_naviframe_item_pop_cb_set(item, callbackDispatcher, this);
	}
};

class Naviframe : public Widget
{
public:
	typedef std::function<void(void*)> Callback;

	Naviframe(Widget *parent);
	virtual ~Naviframe();

	void push(Widget* frame);
	void push(Widget* frame, const std::string& title);
	void pop();
	void addPopCallback(Callback&& callback) {
		top_item = items.top();
		top_item->addCallback(std::move(callback));
	}
private:
	NaviItem *top_item;
	std::stack<NaviItem*> items;
};

#endif
