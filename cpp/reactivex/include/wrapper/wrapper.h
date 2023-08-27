#ifndef RX_WRAPPER_WRAPPER_H
#define RX_WRAPPER_WRAPPER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "wrapper/wrappercast.h"

#define RX_ABSTRACT_WRAPPER(GodotType, AbstractType)                                \
private:                                                                            \
    std::shared_ptr<AbstractType> _ptr;                                             \
public:                                                                             \
    GodotType() { throw NotImplementedException(); }                                \
    GodotType(const std::shared_ptr<AbstractType>& ptr) : _ptr(ptr) {}              \
    ~GodotType(){}                                                                  \
    inline static Ref<GodotType> wrap(const std::shared_ptr<AbstractType>& ptr) {   \
        return ptr ? memnew(GodotType(ptr)) : Ref<GodotType>();                     \
    }                                                                               \
    inline static std::shared_ptr<AbstractType> unwrap(Ref<GodotType> ref) {        \
        return ref.is_null() ? nullptr : ref->_ptr;                                 \
    }                                                                               \
    std::shared_ptr<AbstractType> getptr() const { return this->_ptr; }             \
    inline String _to_string() const {                                              \
        return "[" + String(#GodotType) + ":" + UtilityFunctions::str(              \
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";                    \
    }                                                                               \
    inline bool equals(Ref<GodotType> other) const {                                \
        return this->_ptr.get() == other->_ptr.get();                               \
    }                                                                               \
    inline static Ref<GodotType> dyn_cast(const Variant& input) {                   \
        return dyn_wrapper_cast<AbstractType, GodotType>(input);                    \
    }                                                                               \
    inline static Ref<GodotType> dyn_cast_or_null(const Variant& input) {           \
        return dyn_wrapper_cast_or_null<AbstractType, GodotType>(input);            \
    }                                                                               

#define RX_WRAPPER(GodotType, Type, GodotBaseType, BaseType)                        \
private:                                                                            \
    std::shared_ptr<Type> _ptr;                                                     \
public:                                                                             \
    GodotType() { throw NotImplementedException(); }                                \
    GodotType(const std::shared_ptr<Type>& ptr)                                     \
        : _ptr(ptr), GodotBaseType(std::static_pointer_cast<BaseType>(ptr)) {}      \
    ~GodotType(){}                                                                  \
    inline static Ref<GodotType> wrap(const std::shared_ptr<Type>& ptr) {           \
        return ptr ? memnew(GodotType(ptr)) : Ref<GodotType>();                     \
    }                                                                               \
    inline static std::shared_ptr<Type> unwrap(Ref<GodotType> ref) {                \
        return ref.is_null() ? nullptr : ref->_ptr;                                 \
    }                                                                               \
    std::shared_ptr<BaseType> getptr() const { return this->_ptr; }                 \
    inline String _to_string() const {                                              \
        return "[" + String(#GodotType) + ":" + UtilityFunctions::str(              \
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";                    \
    }                                                                               \
    inline static Ref<GodotType> dyn_cast(const Variant& input) {                   \
        return dyn_wrapper_cast<Type, GodotType>(input);                            \
    }                                                                               \
    inline static Ref<GodotType> dyn_cast_or_null(const Variant& input) {           \
        return dyn_wrapper_cast_or_null<Type, GodotType>(input);                    \
    }                                                                               

#define RX_WRAPPER_CAST_BINDS(Type) \
    ClassDB::bind_static_method(#Type, D_METHOD("dyn_cast", "input"), &Type::dyn_cast); \
    ClassDB::bind_static_method(#Type, D_METHOD("dyn_cast_or_null", "input"), &Type::dyn_cast_or_null); \


#endif // RX_WRAPPER_WRAPPER_H