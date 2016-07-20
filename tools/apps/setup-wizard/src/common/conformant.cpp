#include "conformant.h"

Conformant::Conformant(Widget *parent) : Widget(parent)
{
        nativeObject = elm_conformant_add(nativeParent);
        evas_object_size_hint_weight_set(nativeObject, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_win_resize_object_add(nativeParent, nativeObject);
        show();
}

Conformant::~Conformant()
{
}
