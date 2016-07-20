/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __KLAY_UX_GENLIST_H__
#define __KLAY_UX_GENLIST_H__

#include <memory>
#include <vector>
#include <string>

#include "widget.h"

//namespace klay {

class GenlistLabelProvider {
public:
	virtual std::string getLabel(const std::string& name, const std::string& part, int index) = 0;
};

class GenlistContentProvider {
public:
	virtual Widget* getContent(const std::string& name, const std::string& part, int index) = 0;
};

class GenlistItem {
public:
	void setSelectMode(Elm_Object_Select_Mode mode)
	{
		elm_genlist_item_select_mode_set(item, mode);
	}

	void setDisabled(bool disable)
	{
		elm_object_item_disabled_set(item, disable);
	}

	template <typename LabelProvider, typename ContentProvider> friend class Genlist;
private:
	GenlistItem(Elm_Object_Item *object) :
		item(object)
	{
	}

private:
	Elm_Object_Item *item;
};

template <typename LabelProvider, typename ContentProvider>
class Genlist : public Widget {
public:
	Genlist(Widget *parent)
	{
		labelProvider = new LabelProvider();
		contentProvider = new ContentProvider(this);

		nativeObject = elm_genlist_add(Widget::object(parent));
		setStyle("solid/default");
		elm_genlist_mode_set(nativeObject, ELM_LIST_COMPRESS);
	}

	virtual ~Genlist() {}

	GenlistItem append(const std::string& name, const std::string& style, Elm_Object_Select_Mode mode, int index)
	{
		Elm_Genlist_Item_Class *klass = elm_genlist_item_class_new();
		klass->item_style = style.c_str();
		klass->func.text_get = &Genlist<LabelProvider,ContentProvider>::textHandlerDispatcher;
		klass->func.content_get = &Genlist<LabelProvider,ContentProvider>::contentHandlerDispatcher;

		GenlistItemData* data = new GenlistItemData(name, this, index);

		GenlistItem item(elm_genlist_item_append(nativeObject, klass, data, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL));
		item.setSelectMode(mode);

		return item;
	}

	GenlistLabelProvider* getLabelProvider() const
	{
		return labelProvider;
	}

	GenlistContentProvider* getContentProvider() const
	{
		return contentProvider;
	}

private:
	static char *textHandlerDispatcher(void *data, Evas_Object *obj, const char *part)
	{
		GenlistItemData* self = reinterpret_cast<GenlistItemData *>(data);
		return strdup(self->getLabel(self->name, part, self->index).c_str());
	}

	static Evas_Object *contentHandlerDispatcher(void *data, Evas_Object *obj, const char *part)
	{
		GenlistItemData* self = reinterpret_cast<GenlistItemData *>(data);
		return *self->getContent(self->name, part, self->index);
	}

private:
	struct GenlistItemData {
		GenlistItemData(const std::string& n, Genlist* g, int i) :
			name(n), genlist(g), index(i)
		{
		}

		std::string getLabel(const std::string& name, const std::string& part, int index)
		{
			return genlist->getLabelProvider()->getLabel(name, part, index);
		}

		Widget* getContent(const std::string& name, const std::string& part, int index)
		{
			return genlist->getContentProvider()->getContent(name, part, index);
		}

		std::string name;
		Genlist* genlist;
		int index;
	};

	LabelProvider* labelProvider;
	ContentProvider* contentProvider;
};

//} // namespace klay

#endif //!__KLAY_UX_GENLIST_H__
