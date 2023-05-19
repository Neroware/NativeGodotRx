#include "weakkeydictionary.h"

void WeakKeyDictionary::_bind_methods() {
    ClassDB::bind_static_method("WeakKeyDictionary", D_METHOD("Get"), &WeakKeyDictionary::Get);
    ClassDB::bind_method(D_METHOD("set_pair", "key", "value"), &WeakKeyDictionary::set_pair);
    ClassDB::bind_method(D_METHOD("get_value", "key"), &WeakKeyDictionary::get_value);
    ClassDB::bind_method(D_METHOD("find_key", "key"), &WeakKeyDictionary::find_key);
    ClassDB::bind_method(D_METHOD("is_empty"), &WeakKeyDictionary::is_empty);
    ClassDB::bind_method(D_METHOD("clear"), &WeakKeyDictionary::clear);
    ClassDB::bind_method(D_METHOD("erase", "key"), &WeakKeyDictionary::erase);
    ClassDB::bind_method(D_METHOD("values"), &WeakKeyDictionary::values);
    ClassDB::bind_method(D_METHOD("size"), &WeakKeyDictionary::size);
    ClassDB::bind_method(D_METHOD("keys"), &WeakKeyDictionary::keys);
    ClassDB::bind_method(D_METHOD("has_key", "key"), &WeakKeyDictionary::has_key);
    ClassDB::bind_method(D_METHOD("has_all", "keys"), &WeakKeyDictionary::has_all);
    ADD_PROPERTY(PropertyInfo(Variant::Type::INT, "length"), "", "size");
}

Ref<WeakKeyDictionary> WeakKeyDictionary::Get() {
    return memnew(WeakKeyDictionary);
}

void WeakKeyDictionary::set_pair(const Variant& key, const Variant& value) {
    auto okey = CAST_OR_NULL(key, Object);
    if (!okey) {
        throw BadArgumentException("Key needs to be of type 'Object' and not null!");
    }
    auto hkey = this->_hash_key(key);
    auto wkey = REF_CAST(UtilityFunctions::weakref(key), WeakRef);
    auto disp = REF_CAST(UtilityFunctions::weakref(this->_add_disposer(okey, hkey)), WeakRef);
    this->_set_pair(hkey, wkey, value, disp);
}

Variant WeakKeyDictionary::get_value(const Variant& key) const {
    auto hkey = this->_hash_key(key);
    try {
        return std::get<1>(this->_data.at(hkey));
    }
    catch(std::out_of_range) {
        return Variant();
    }
}

Variant WeakKeyDictionary::find_key(const Variant& value) const {
    for (auto pair : this->_data) {
        if (std::get<1>(pair.second) == value) {
            return std::get<0>(pair.second)->get_ref();
        }
    }
    return Variant();
}

bool WeakKeyDictionary::is_empty() const {
    return this->_data.empty();
}

void WeakKeyDictionary::clear() {
    auto keys = this->keys();
    for (auto i = 0ul; i < keys.size(); i++) {
        this->erase(keys[i]);
    }
}

bool WeakKeyDictionary::erase(const Variant& key) {
    auto hkey = this->_hash_key(key);
    try {
        REF_CAST(std::get<2>(this->_data[hkey])->get_ref(), AutoDisposable)->dispose();
        return true;
    }
    catch(std::out_of_range) {
        return false;
    }
}

Array WeakKeyDictionary::values() const {
    Array values;
    for (auto pair : this->_data) {
        values.push_back(std::get<1>(pair.second));
    }
    return values;
}

size_t WeakKeyDictionary::size() const {
    return this->_data.size();
}

Array WeakKeyDictionary::keys() const {
    Array keys;
    for (auto pair : this->_data) {
        keys.push_back(OBJ_CAST(std::get<0>(pair.second)->get_ref(), Object));
    }
    return keys;
}

bool WeakKeyDictionary::has_key(const Variant& key) const {
    return this->_data.find(this->_hash_key(key)) != this->_data.end();
}

bool WeakKeyDictionary::has_all(const Array& keys) const {
    for (auto i = 0ul; i < keys.size(); i++) {
        if (!has_key(keys[i])) 
            return false;
    }
    return true;
}