#include "iterator.h"

#include "internal/tuple.h"

void ArrayIterator::_bind_methods() {
    ClassDB::bind_static_method("ArrayIterator", D_METHOD("Get", "array"), &ArrayIterator::Get);
    ClassDB::bind_method(D_METHOD("next"), &ArrayIterator::next);
    ClassDB::bind_method(D_METHOD("has_next"), &ArrayIterator::has_next);
    ClassDB::bind_method(D_METHOD("front"), &ArrayIterator::front);
    ClassDB::bind_method(D_METHOD("back"), &ArrayIterator::back);
    ClassDB::bind_method(D_METHOD("end"), &ArrayIterator::end);
    ClassDB::bind_method(D_METHOD("empty"), &ArrayIterator::empty);
}
Ref<ArrayIterator> ArrayIterator::Get(const Array& array) {
    return memnew(ArrayIterator(array));
}
Variant ArrayIterator::next() {
    return !this->has_next() ? this->_end : this->_array[this->_count++];
}
bool ArrayIterator::has_next() {
    return this->_count < this->_array.size();
}
Variant ArrayIterator::front() {
    return this->empty() ? this->_end : this->_array[0];
}
Variant ArrayIterator::back() {
    return this->empty() ? this->_end : this->_array[this->_array.size() - 1];
}
Variant ArrayIterator::end() {
    return this->_end;
}
bool ArrayIterator::empty() {
    return this->_array.is_empty();
}


void DictionaryIterator::_bind_methods() {
    ClassDB::bind_static_method("DictionaryIterator", D_METHOD("Get", "dict"), &DictionaryIterator::Get);
    ClassDB::bind_method(D_METHOD("next"), &DictionaryIterator::next);
    ClassDB::bind_method(D_METHOD("has_next"), &DictionaryIterator::has_next);
    ClassDB::bind_method(D_METHOD("front"), &DictionaryIterator::front);
    ClassDB::bind_method(D_METHOD("back"), &DictionaryIterator::back);
    ClassDB::bind_method(D_METHOD("end"), &DictionaryIterator::end);
    ClassDB::bind_method(D_METHOD("empty"), &DictionaryIterator::empty);
}
Ref<DictionaryIterator> DictionaryIterator::Get(const Dictionary& dict) {
    return memnew(DictionaryIterator(dict));
}
Variant DictionaryIterator::next() {
    return !this->has_next() ? this->_end : Variant(Tuple::Get(Array::make(
        this->_keys[this->_count++], 
        this->_dict[this->_keys[this->_count]]
    )));
}
bool DictionaryIterator::has_next() {
    return this->_count < this->_keys.size();
}
Variant DictionaryIterator::front() {
    return this->empty() ? this->_end : Variant(Tuple::Get(Array::make(
        this->_keys[0], 
        this->_dict[this->_keys[0]]
    )));
}
Variant DictionaryIterator::back() {
    return this->empty() ? this->_end : Variant(Tuple::Get(Array::make(
        this->_keys[this->_keys.size() - 1], 
        this->_dict[this->_keys[this->_keys.size() - 1]]
    )));
}
Variant DictionaryIterator::end() {
    return this->_end;
}
bool DictionaryIterator::empty() {
    return this->_keys.is_empty();
}