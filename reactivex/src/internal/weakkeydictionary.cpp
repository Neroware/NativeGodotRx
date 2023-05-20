#include "weakkeydictionary.h"

#define SAFE_READ std::shared_lock<std::shared_mutex> readLock(*_lock);
#define SAFE_WRITE std::unique_lock<std::shared_mutex> writeLock(*_lock);

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
    SAFE_WRITE
        auto okey = CAST_OR_NULL(key, Object);
        if (!okey) {
            throw BadArgumentException("Key needs to be of type 'Object' and not null!");
        }
        auto hkey = this->_hash_key(key);
        try {
            KeyEntry entry = this->_data.at(hkey);
            this->_set_pair(hkey, std::get<0>(entry), value, std::get<2>(entry));
        }
        catch (std::out_of_range) {
            auto wkey = REF_CAST(UtilityFunctions::weakref(key), WeakRef);
            auto disp = REF_CAST(UtilityFunctions::weakref(this->_create_disposer(okey, hkey)), WeakRef);
            this->_set_pair(hkey, wkey, value, disp);
        }
}

Variant WeakKeyDictionary::get_value(const Variant& key) const {
    SAFE_READ
        auto hkey = this->_hash_key(key);
        try {
            return std::get<1>(this->_data.at(hkey));
        }
        catch(std::out_of_range) {
            return Variant();
        }
}

Variant WeakKeyDictionary::find_key(const Variant& value) const {
    SAFE_READ
        for (auto pair : this->_data) {
            if (std::get<1>(pair.second) == value) {
                return std::get<0>(pair.second)->get_ref();
            }
        }
        return Variant();
}

bool WeakKeyDictionary::is_empty() const {
    SAFE_READ return this->_data.empty();
}

bool WeakKeyDictionary::erase(const Variant& key) {
    SAFE_WRITE
        auto hkey = this->_hash_key(key);
        try {
            KeyEntry entry = this->_data.at(hkey);
            if (Object* key = CAST_OR_NULL(std::get<0>(entry)->get_ref(), Object)) {
                REF_CAST(std::get<2>(entry)->get_ref(), AutoDisposable)->cancel(key);
                this->_remove_pair(hkey);
            }
            return true;
        }
        catch(std::out_of_range) {
            return false;
        }
}

void WeakKeyDictionary::clear() {
    SAFE_WRITE
        Array hkeys;
        for (auto pair : this->_data) {
            hkeys.push_back(pair.first);
        }
        for (auto i = 0ul; i < hkeys.size(); i++) {
            auto hkey = hkeys[i];
            KeyEntry entry = this->_data.at(hkey);
            if (Object* key = CAST_OR_NULL(std::get<0>(entry)->get_ref(), Object)) {
                REF_CAST(std::get<2>(entry)->get_ref(), AutoDisposable)->cancel(key);
                this->_remove_pair(hkey);
            }
        }
}

Array WeakKeyDictionary::values() const {
    SAFE_READ
        Array values;
        for (auto pair : this->_data) {
            values.push_back(std::get<1>(pair.second));
        }
        return values;
}

size_t WeakKeyDictionary::size() const {
    SAFE_READ return this->_data.size();
}

Array WeakKeyDictionary::keys() const {
    SAFE_READ
        Array keys;
        for (auto pair : this->_data) {
            keys.push_back(OBJ_CAST(std::get<0>(pair.second)->get_ref(), Object));
        }
        return keys;
}

bool WeakKeyDictionary::has_key(const Variant& key) const {
    SAFE_READ return this->_data.find(this->_hash_key(key)) != this->_data.end();
}

bool WeakKeyDictionary::has_all(const Array& keys) const {
    SAFE_READ
        for (auto i = 0ul; i < keys.size(); i++) {
            if (!has_key(keys[i])) 
                return false;
        }
        return true;
}