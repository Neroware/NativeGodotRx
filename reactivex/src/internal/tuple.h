#ifndef RX_TUPLE_H
#define RX_TUPLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "abstract/iterable.h"

using namespace godot;

class Tuple : public IterableBase {
    GDCLASS(Tuple, IterableBase);


private:
    Array _data;

protected:
	static void _bind_methods();

public:
    Tuple(){}
    Tuple(const Array& data) : _data(data) {}
    ~Tuple(){}

    static Ref<Tuple> Get(const Array& data);
    static Ref<Tuple> Empty();

    uint64_t size() const;
    Variant at(uint64_t index) const;
    void setat(uint64_t index, const Variant& what);
    Array to_list() const;
    String _to_string() const;
    Ref<IteratorBase> iter() override;

};

#endif // RX_TUPLE_H