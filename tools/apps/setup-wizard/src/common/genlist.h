#ifndef __KLAY_GENLIST_H__
#define __KLAY_GENLIST_H__
#include <memory>
#include <vector>
#include "widget.h"

class GenlistItemClass : public Widget {
public:
	typedef std::unordered_map<std::string, std::string> TextInfo;
	typedef std::unordered_map<std::string, Evas_Object *> ContentInfo;

	GenlistItemClass(const std::string &style) : nativeStyle(style)
	{
		create();
	}
	virtual ~GenlistItemClass()
	{
		free();
	}

	void create(void)
	{
		item_class = elm_genlist_item_class_new();
		item_class->item_style = nativeStyle.c_str();
		item_class->func.text_get = &GenlistItemClass::textHandlerDispatcher;
		item_class->func.content_get = &GenlistItemClass::contentHandlerDispatcher;
	}

	void addText(const std::string &part, const std::string &text)
	{
		textInfo[part] = text;
	}

	void addContent(const std::string &part, Widget *content)
	{
		contentInfo[part] = Widget::object(content);
	}

	void free(void)
	{
		elm_genlist_item_class_free(item_class);
	}

	friend class Genlist;

private:
	static char *textHandlerDispatcher(void *data, Evas_Object *obj, const char *part)
	{
		GenlistItemClass *instance = (GenlistItemClass *) data;
		if (instance->textInfo.find(part) != instance->textInfo.end())
			return strdup(instance->textInfo[part].c_str());
		else
			return NULL;
	}
	static Evas_Object *contentHandlerDispatcher(void *data, Evas_Object *obj, const char *part)
	{
		GenlistItemClass *instance = (GenlistItemClass *) data;
		if (instance->contentInfo.find(part) != instance->contentInfo.end())
			return instance->contentInfo[part];
		else
			return NULL;
	}

	TextInfo textInfo;
	ContentInfo contentInfo;

	std::string nativeStyle;
	Elm_Genlist_Item_Class *item_class;
};

class GenlistItem {
public:
	void setSelectMode(Elm_Object_Select_Mode mode)
	{
		elm_genlist_item_select_mode_set(item, mode);
	}

	friend class Genlist;
private:
	GenlistItem(Elm_Object_Item *object) : item(object) {}
private:
	Elm_Object_Item *item;
};

class Genlist : public Widget {
public:
	typedef std::unique_ptr<GenlistItemClass> GenlistItemClassPtr;
	typedef std::vector<GenlistItemClassPtr> ItemClassList;

	Genlist(Widget *parent)
	{
		nativeObject = elm_genlist_add(Widget::object(parent));
		setStyle("solid/default");
		elm_genlist_mode_set(nativeObject, ELM_LIST_COMPRESS);
	}
	virtual ~Genlist() {}

	GenlistItem append(GenlistItemClass *itc)
	{
		return appendInternal(itc);
	}
	GenlistItem append(GenlistItemClass *itc, Elm_Object_Select_Mode mode)
	{
		GenlistItem item = appendInternal(itc);
		item.setSelectMode(mode);
		return item;
	}
private:
	GenlistItem appendInternal(GenlistItemClass *itc)
	{
		GenlistItemClass *newitc = new GenlistItemClass(*itc);
		itemClassList.push_back(GenlistItemClassPtr(newitc));
		GenlistItem item(elm_genlist_item_append(nativeObject, itc->item_class, newitc, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL));
		return item;
	}
protected:
	ItemClassList itemClassList;
};

#endif
