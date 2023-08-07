#ifndef RX_WRAPPER_WRAPPER_H
#define RX_WRAPPER_WRAPPER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#define RX_ABSTRACT_WRAPPER(GodotType, AbstractType)                                \
private:                                                                            \
    std::shared_ptr<AbstractType> _ptr;                                             \
public:                                                                             \
    GodotType() { throw NotImplementedException(); }                                \
    GodotType(const std::shared_ptr<AbstractType>& ptr) : _ptr(ptr) {}              \
    ~GodotType(){}                                                                  \
    static Ref<GodotType> wrap(const std::shared_ptr<AbstractType>& ptr) {          \
        return memnew(GodotType(ptr));                                              \
    }                                                                               \
    std::shared_ptr<AbstractType> unwrap() const { return this->_ptr; }             \
    inline String _to_string() const {                                              \
        return "[Rx" + String(#GodotType) + ":" + UtilityFunctions::str(            \
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";                    \
    }                                                                               \
    inline bool equals(Ref<GodotType> other) const {                                \
        return this->_ptr.get() == other->_ptr.get();                               \
    }

#define RX_WRAPPER(GodotType, Type, BaseType)                                       \
private:                                                                            \
    std::shared_ptr<Type> _ptr;                                                     \
public:                                                                             \
    GodotType() { throw NotImplementedException(); }                                \
    GodotType(const std::shared_ptr<Type>& ptr) : _ptr(ptr), BaseType(ptr) {}       \
    ~GodotType(){}                                                                  \
    static Ref<GodotType> wrap(const std::shared_ptr<AbstractType>& ptr) {          \
        return memnew(GodotType(ptr));                                              \
    }                                                                               \
    std::shared_ptr<Type> unwrap() const { return this->_ptr; }                     \
    inline String _to_string() const {                                              \
        return "[Rx" + String(#GodotType) + ":" + UtilityFunctions::str(            \
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";                    \
    }                                                                               


#endif // RX_WRAPPER_WRAPPER_H