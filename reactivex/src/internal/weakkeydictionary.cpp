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
    auto wkey = CAST(UtilityFunctions::weakref(key), WeakRef);
    auto disp = CAST(UtilityFunctions::weakref(this->_add_disposer(okey, hkey)), WeakRef);
    this->_set_pair(hkey, wkey, disp, value);
}

Variant WeakKeyDictionary::get_value(const Variant& key) const {
    auto hkey = this->_hash_key(key);
    try {
        return this->_values.at(hkey);
    }
    catch(std::out_of_range) {
        return Variant();
    }
}

Variant WeakKeyDictionary::find_key(const Variant& value) const {
    for (auto pair : this->_values) {
        if (pair.second == value) {
            return this->_weakkeys.at(pair.first).first->get_ref();
        }
    }
    return Variant();
}

bool WeakKeyDictionary::is_empty() const {
    return this->_values.empty();
}

void WeakKeyDictionary::clear() {
    for (auto pair : this->_weakkeys) {
        CAST(pair.second.second->get_ref(), AutoDisposable)->dispose();
    }
}

bool WeakKeyDictionary::erase(const Variant& key) {
    auto hkey = this->_hash_key(key);
    try {
        CAST(this->_weakkeys[hkey].second->get_ref(), AutoDisposable)->dispose();
        return true;
    }
    catch(std::out_of_range) {
        return false;
    }
}

Array WeakKeyDictionary::values() const {
    Array values;
    for (auto pair : this->_values) {
        values.push_back(pair.second);
    }
    return values;
}

size_t WeakKeyDictionary::size() const {
    return this->_values.size();
}

Array WeakKeyDictionary::keys() const {
    Array keys;
    for (auto pair : this->_weakkeys) {
        auto wref = pair.second.first;
        if (auto key = CAST_OR_NULL(wref->get_ref(), RefCounted)) {
            keys.push_back(Ref<RefCounted>(key));
        }
        else if (auto key = CAST_OR_NULL(wref->get_ref(), Object)) {
            keys.push_back(key);
        }
    }
    return keys;
}

bool WeakKeyDictionary::has_key(const Variant& key) const {
    auto hkey = this->_hash_key(key);
    return this->_values.find(key) != this->_values.end();
}

bool WeakKeyDictionary::has_all(const Array& keys) const {
    for (auto i = 0ul; i < keys.size(); i++) {
        if (!has_key(keys[i])) 
            return false;
    }
    return true;
}