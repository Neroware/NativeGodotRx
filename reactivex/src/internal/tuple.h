#ifndef RX_TUPLE_H
#define RX_TUPLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class Tuple : public RefCounted {
    GDCLASS(Tuple, RefCounted);


private:
    Array _data;

protected:
	static void _bind_methods() {
        ClassDB::bind_static_method("Tuple", D_METHOD("Get", "data"), &Tuple::Get);
        ClassDB::bind_static_method("Tuple", D_METHOD("Empty"), &Tuple::Empty);
        ClassDB::bind_method(D_METHOD("size"), &Tuple::size);
        ClassDB::bind_method(D_METHOD("at", "index"), &Tuple::at);
        ClassDB::bind_method(D_METHOD("setat", "index", "what"), &Tuple::setat);
        ClassDB::bind_method(D_METHOD("to_list"), &Tuple::to_list);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "length"), "", "size");
    }

public:
    Tuple(){}
    Tuple(const Array& data) : _data(data) {}
    Tuple(const Variant& data...) : _data(data) {}
    ~Tuple(){}

    static inline Ref<Tuple> Get(const Array& data) {
        return memnew(Tuple(data));
    }
    static inline Ref<Tuple> Empty() {
        return memnew(Tuple);
    }
    static inline Ref<Tuple> Pack(const Variant& data...) {
        return memnew(Tuple(data));
    }

    inline uint64_t size() const {
        return this->_data.size();
    }

    inline Variant at(uint64_t index) const {
        return this->_data[index];
    }

    inline void setat(uint64_t index, const Variant& what) {
        this->_data[index] = what;
    }

    inline Array to_list() const {
        return this->_data.duplicate();
    }

    inline String _to_string() const {
        String res = "( ";
        for (auto i = 0ul; i < this->_data.size(); i++) {
            res += UtilityFunctions::var_to_str(this->_data[i]) + " ";
        }
        res += ")";
        return res;
    }

};

#endif // RX_TUPLE_H