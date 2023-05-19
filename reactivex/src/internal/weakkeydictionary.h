#ifndef RX_WEAKKEYDICTIONARY_H
#define RX_WEAKKEYDICTIONARY_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/weak_ref.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <unordered_map>

#include "internal/basic.h"
#include "disposable/autodisposable.h"
#include "exception/exception.h"

using namespace godot;

class WeakKeyDictionary : public RefCounted {
    GDCLASS(WeakKeyDictionary, RefCounted);

    using KeyEntry = std::tuple<Ref<WeakRef>, Variant, Ref<WeakRef>>;

private:
    std::unordered_map<int64_t, KeyEntry> _data;

    static inline int64_t _hash_key(const Variant& key) {
        return UtilityFunctions::hash(key);
    }

    inline Ref<AutoDisposable> _add_disposer(Object* key, int64_t hkey) const {
        Variant ref = UtilityFunctions::weakref(this);
        auto ad = memnew(AutoDisposable([ref, hkey](){
            Ref<WeakRef> _ref = REF_CAST(ref, WeakRef);
            if (auto _dict = CAST_OR_NULL(_ref->get_ref(), WeakKeyDictionary)) {
                _dict->_remove_pair(hkey);
            }
        }));
        ad->dispose_with(key);
        return ad;
    }

    inline void _set_pair(int64_t hkey, Ref<WeakRef> wkey, const Variant& value, Ref<WeakRef> disp) {
        this->_data[hkey] = std::make_tuple(wkey, value, disp);
    }

    inline bool _remove_pair(int64_t hkey) {
        return this->_data.erase(hkey);
    }

protected:
	static void _bind_methods();

public:
    WeakKeyDictionary(){}
    ~WeakKeyDictionary(){}

    static Ref<WeakKeyDictionary> Get();

    void set_pair(const Variant& key, const Variant& value);
    Variant get_value(const Variant& key) const;
    Variant find_key(const Variant& key) const;
    bool is_empty() const;
    void clear();
    bool erase(const Variant& key);
    Array values() const;
    size_t size() const;
    Array keys() const;
    bool has_key(const Variant& key) const;
    bool has_all(const Array& keys) const;

};

#endif // RX_WEAKKEYDICTIONARY_H