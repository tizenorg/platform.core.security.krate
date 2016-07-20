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

#include <cstring>
#include <memory>
#include <vector>
#include <string>

#include "widget.h"
#include "radio.h"
namespace klay {

class Genlist;

class GenlistContentProvider {
public:
	virtual char* getLabel(const std::string& name, const std::string& part) = 0;
	virtual Widget* getWidget(Genlist* parent, const std::string& name, const std::string& part) = 0;
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

	friend class Genlist;

private:
	GenlistItem(Elm_Object_Item *object) :
		item(object)
	{
	}

private:
	Elm_Object_Item *item;
};

class Genlist : public Widget {
public:
	Genlist(Widget *parent, GenlistContentProvider *provider) :
		contentProvider(provider)
	{
		nativeObject = elm_genlist_add(Widget::object(parent));
		setStyle("solid/default");
		elm_genlist_mode_set(nativeObject, ELM_LIST_COMPRESS);
	}

	virtual ~Genlist() {}

	GenlistItem append(const std::string& name, const std::string& style, Elm_Object_Select_Mode mode = ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY)
	{
		Elm_Genlist_Item_Class *klass = elm_genlist_item_class_new();
		klass->item_style = strdup(style.c_str());
		klass->func.text_get = &Genlist::textHandlerDispatcher;
		klass->func.content_get = &Genlist::contentHandlerDispatcher;

		GenlistItemData* data = new GenlistItemData(name, this);

		GenlistItem item(elm_genlist_item_append(nativeObject, klass, data, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL));
		item.setSelectMode(mode);
		elm_genlist_item_class_free(klass);

		return item;
	}

	void setContentProvider(GenlistContentProvider* provider)
	{
		contentProvider = provider;
	}

	GenlistContentProvider* getContentProvider() const
	{
		return contentProvider;
	}

private:
	static char *textHandlerDispatcher(void *data, Evas_Object *obj, const char *part)
	{
		GenlistItemData* self = reinterpret_cast<GenlistItemData *>(data);
		return self->getLabel(self->name, part);
	}

	static Evas_Object *contentHandlerDispatcher(void *data, Evas_Object *obj, const char *part)
	{
		GenlistItemData* self = reinterpret_cast<GenlistItemData *>(data);
		Widget* widget = self->getWidget(self->genlist, self->name, part);
		if (widget != nullptr)
			return Widget::object(widget);
		return nullptr;
	}

private:
	struct GenlistItemData {
		GenlistItemData(const std::string& n, Genlist* g) :
			name(n), genlist(g)
		{
		}

		char* getLabel(const std::string& name, const std::string& part)
		{
			GenlistContentProvider *contentProvider = genlist->getContentProvider();
			if (contentProvider) {
				return contentProvider->getLabel(name, part);
			}
			dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "No label provider has been applied\n");
			return nullptr;
		}

		Widget* getWidget(Genlist* parent, const std::string& name, const std::string& part)
		{
			GenlistContentProvider* contentProvider = genlist->getContentProvider();
			if (contentProvider) {
				return contentProvider->getWidget(parent, name, part);
			}
			dlog_print(DLOG_ERROR, KLAY_LOG_TAG, "No content provider has been applied\n");
			return nullptr;
		}

		std::string name;
		Genlist* genlist;
	};

	GenlistContentProvider* contentProvider;
};

} // namespace klay

#endif //!__KLAY_UX_GENLIST_H__
