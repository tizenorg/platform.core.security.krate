#ifndef __KLAY_ENTRY_H__
#define __KLAY_ENTRY_H__

#include "widget.h"

class Entry : public Widget {
public:
	Entry(Widget *parent) {
		nativeObject = elm_entry_add(Widget::object(parent));
		evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(entry);
	}
	virtual ~Entry() {}

	void setSingleLine(bool enable) {
		elm_entry_single_line_set(nativeObject, enable);
	}

	void setPassword(bool enable) {
		elm_entry_password_set(nativeObject, enable);
	}

	void setLimitFilter(int max_char_count, int max_byte_count) {
		Elm_Entry_Filter_Limit_Size limit_size = {
			.max_char_count = max_char_count,
			.max_byte_count = max_byte_count
		};
		elm_entry_markup_filter_append(nativeObject, elm_entry_filter_limit_size, &limit_size);
	}

	void set(const std::string& text) {
		elm_entry_entry_set(nativeObject, text.c_str());
	}

	std::string get() const {
		return elm_entry_entry_get(nativeObject);
	}

	bool isEmpty() {
		return elm_entry_is_empty(nativeObject);
	}
};

#endif /*__KLAY_ENTRY_H__*/
