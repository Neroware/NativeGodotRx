#ifndef RX_CAST_H
#define RX_CAST_H

#define DEFAULT_ON_NEXT rx::basic::noop<const Variant&>
#define DEFAULT_ON_ERROR rx::basic::noop<const std::exception&>
#define DEFAULT_ON_COMPLETED rx::basic::noop<>
#define DEFAULT_DISPOSE rx::basic::noop<>

#define DYN_CAST(x, cls) Object::cast_to<cls>(x)
#define DYN_CAST_OR_NULL(x, cls) x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls) ? Object::cast_to<cls>(x) : nullptr
#define IS_CLASS(x, cls) (x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls))

#endif // RX_CAST_H