#include "internal/utils.h"

#include "godotrx.h"
#include "cast.h"

namespace rx {

bool NotSet::is(const Variant& v) {
    return IS_CLASS(v, NotSet);
}

Ref<NotSet> NotSet::value() {
    return GDRX->NOT_SET;
}

}