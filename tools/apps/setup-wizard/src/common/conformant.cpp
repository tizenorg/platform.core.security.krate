#include "conformant.h"

Conformant::Conformant(Widget *parent)
{
    nativeObject = elm_conformant_add(Widget::object(parent));
    evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(Widget::object(parent), nativeObject);
}

Conformant::~Conformant()
{
}
