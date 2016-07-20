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
	GenlistContentProvider();
	~GenlistContentProvider();

	virtual char* getLabel(const std::string& name, const std::string& part) = 0;
	virtual Widget* getWidget(Genlist* parent, const std::string& name, const std::string& part) = 0;
};

class GenlistItem {
public:
	~GenlistItem();

	void setSelectMode(Elm_Object_Select_Mode mode);
	void setDisabled(bool disable);

	friend class Genlist;

private:
	GenlistItem(Elm_Object_Item *object);

private:
	Elm_Object_Item *item;
};

class Genlist : public Widget {
public:
	enum Flags {
		setCompressMode = (1 << 0),
		setListScrollMode = (1 << 1),
		setHomogeneous = (1 << 2),
	};

	Genlist(Widget *parent, GenlistContentProvider *provider, int style);
	virtual ~Genlist();

	GenlistItem append(const std::string& name, const std::string& style);

	void setContentProvider(GenlistContentProvider* provider);
	GenlistContentProvider* getContentProvider() const;

private:
	static char *textHandlerDispatcher(void *data, Evas_Object *obj, const char *part);
	static Evas_Object *contentHandlerDispatcher(void *data, Evas_Object *obj, const char *part);

private:
	struct GenlistItemData {
		GenlistItemData(const std::string& itemName, Genlist* parent);
		~GenlistItemData();

		char* getLabel(const std::string& name, const std::string& part);
		Widget* getWidget(Genlist* parent, const std::string& name, const std::string& part);

		std::string name;
		Genlist* genlist;
	};

	GenlistContentProvider* contentProvider;
};

} // namespace klay

#endif //!__KLAY_UX_GENLIST_H__
