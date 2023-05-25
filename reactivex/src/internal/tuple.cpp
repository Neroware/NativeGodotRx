#include "tuple.h"

#include "internal/iterator.h"

void Tuple::_bind_methods() {
    ClassDB::bind_static_method("Tuple", D_METHOD("Get", "data"), &Tuple::Get);
    ClassDB::bind_static_method("Tuple", D_METHOD("Empty"), &Tuple::Empty);
        ClassDB::bind_method(D_METHOD("size"), &Tuple::size);
    ClassDB::bind_method(D_METHOD("at", "index"), &Tuple::at);
    ClassDB::bind_method(D_METHOD("setat", "index", "what"), &Tuple::setat);
    ClassDB::bind_method(D_METHOD("to_list"), &Tuple::to_list);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "length"), "", "size");
}

Ref<Tuple> Tuple::Get(const Array& data) {
    return memnew(Tuple(data));
}
Ref<Tuple> Tuple::Empty() {
    return memnew(Tuple);
}
uint64_t Tuple::size() const {
    return this->_data.size();
}
Variant Tuple::at(uint64_t index) const {
    return this->_data[index];
}
void Tuple::setat(uint64_t index, const Variant& what) {
    this->_data[index] = what;
}
Array Tuple::to_list() const {
    return this->_data.duplicate();
}
String Tuple::_to_string() const {
    String res = "(";
    for (auto i = 0ul; i < this->_data.size(); i++) {
        if (i > 0) res += ", ";
        res += UtilityFunctions::var_to_str(this->_data[i]);
    }
    res += ")";
    return res;
}
Ref<IteratorBase> Tuple::iter() {
    return ArrayIterator::Get(this->to_list());
}