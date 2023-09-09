#include "internal/utils.h"

#include "godotrx.h"

using namespace godot;

namespace rx {

bool NotSet::is(const Variant& v) {
    return IS_CLASS(v, NotSet);
}

Ref<NotSet> NotSet::value() {
    return GDRX->NOT_SET;
}

}