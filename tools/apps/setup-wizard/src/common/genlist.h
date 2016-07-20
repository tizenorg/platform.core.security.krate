#ifndef __KLAY_GENLIST_H__
#define __KLAY_GENLIST_H__

#include "widget.h"

class GenlistItemClass : public Widget {
public:
	typedef std::unordered_map<std::string, std::string> TextInfo;
	typedef std::unordered_map<std::string, Evas_Object*> ContentInfo;

	GenlistItemClass(const std::string& style) : nativeStyle(style) {
		item_class = elm_genlist_item_class_new();
		item_class->item_style = nativeStyle.c_str();
		item_class->func.text_get = &this->textHandlerDispatcher;
		item_class->func.content_get = &this->contentHandlerDispatcher;
	}

	void addText(const std::string& part, const std::string& text) {
		textInfo[part] = text;
	}

	void addContent(const std::string& part, Widget* content) {
		contentInfo[part] = Widget::object(content);
	}

	void free() {
		elm_genlist_item_class_free(item_class);
	}

	friend class Genlist;
private:
	static char* textHandlerDispatcher(void* data, Evas_Object *obj, const char* part) {
		GenlistItemClass *instance = (GenlistItemClass*) data;
		return strdup(instance->textInfo[part].c_str());
	}
	static Evas_Object* contentHandlerDispatcher(void* data, Evas_Object *obj, const char* part) {
		GenlistItemClass *instance = (GenlistItemClass*) data;
		return instance->contentInfo[part];
	}

	TextInfo textInfo;
	ContentInfo contentInfo;

	std::string nativeStyle;
	Elm_Genlist_Item_Class *item_class;
};

class GenlistItem {
public:
	void setSelectMode(Elm_Object_Select_Mode mode) {
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
	Genlist(Widget *parent) {
		nativeObject = elm_genlist_add(Widget::object(parent));
		setStyle("solid/default");
		elm_genlist_mode_set(nativeObject, ELM_LIST_COMPRESS);
	}
	virtual ~Genlist() {}

	GenlistItem append(GenlistItemClass *itc) {
		GenlistItem item(elm_genlist_item_append(nativeObject, itc->item_class, itc, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL));
		return item;
	}
};

#endif
