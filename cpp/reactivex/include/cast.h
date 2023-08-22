#ifndef RX_CAST_H
#define RX_CAST_H

#define DYN_CAST(x, cls) Object::cast_to<cls>(x)
#define DYN_CAST_OR_NULL(x, cls) x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls) ? Object::cast_to<cls>(x) : nullptr
#define IS_CLASS(x, cls) (x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls))
#define IS_TYPE(x, type) (x.get_type() == Variant::Type::type)
#define IS_NUMBER(x) (x.get_type() == Variant::Type::INT || x.get_type() == Variant::Type::FLOAT)

#endif // RX_CAST_H