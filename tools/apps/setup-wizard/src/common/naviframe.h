#ifndef __KLAY_NAVIFRAME_H__
#define __KLAY_NAVIFRAME_H__

#include "widget.h"
#include <stack>

class NaviItem : public WidgetItem
{
public:
	NaviItem(Widget* mainFrame, Widget *content);
	NaviItem(Widget* mainFrame, Widget *content, const std::string& title);
	
	void addCallback(Callback&& callback);
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
