#ifndef RX_ITERATOR_H
#define RX_ITERATOR_H

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "wrapper/abstract.h"
#include "typing.h"
#include "cast.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::wrappers;

namespace rx {

class ItEnd : public RefCounted {
    GDCLASS(ItEnd, RefCounted)

protected:
    static inline void _bind_methods() {}

public:
    ItEnd() {}
    ~ItEnd() {}

    inline String _to_string() const {
	    return "<<ItEnd>>";
    }
};

struct array_iterator : public IteratorBase {
    Array array;
    uint64_t index;
    Ref<ItEnd> itEnd;

    explicit array_iterator(const Array& array_, uint64_t index_ = 0) 
        : array(array_), index(index_), itEnd(memnew(ItEnd)) {}
    
    inline Variant next() override {
        return index >= array.size() ? Variant(this->itEnd) : array[index++];
    }
    inline bool has_next() override {
        return index < array.size();
    }
}; // END array_iterator

struct array_iterable : public IterableBase {
    Array array;

    array_iterable(const Array& array_) 
        : array(array_) {}

    inline iterator_t iter() override {
        return std::make_shared<array_iterator>(array);
    }
}; // END array_iterable

struct dictionary_iterator : public IteratorBase {
    Dictionary dict;
    Array keys;
    uint64_t index;
    Ref<ItEnd> itEnd;

    explicit dictionary_iterator(const Dictionary& dict_, uint64_t index_ = 0) 
        : dict(dict_), keys(dict.keys()), index(index_), itEnd(memnew(ItEnd)) {}
    
    inline Variant next() override {
        return index >= keys.size() ? Variant(this->itEnd) : Variant(Array::make(keys[index], dict[keys[index++]]));
    }
    inline bool has_next() override {
        return index < keys.size();
    }
}; // END dictionary_iterator

struct dictionary_iterable : public IterableBase {
    Dictionary dict;

    dictionary_iterable(const Dictionary& dict_) 
        : dict(dict_) {}

    inline iterator_t iter() override {
        return std::make_shared<dictionary_iterator>(dict);
    }
}; // END dictionary_iterable

struct infinite_iterable : public IterableBase {

    Variant value;

    struct infinite_iterator : public IteratorBase {

        Variant value;
        uint64_t counter = 0;

        infinite_iterator(const Variant& value_)
            : value(value_) {}
        
        inline Variant next() override {
            return value ? value : Variant(counter++);
        }

        inline bool has_next() override {
            return true;
        }

    }; // END infinite_iterator

    infinite_iterable(const Variant& value_ = VNULL)
        : value(value_) {}
    
    inline iterator_t iter() override {
        return std::make_shared<infinite_iterator>(this->value);
    }

}; // END infinite_iterable

struct while_iterable : public IterableBase {

    iterable_t it;
    predicate_t<> pred;

    struct while_iterator : public IteratorBase {

        iterator_t it;
        predicate_t<> pred;
        Variant end;

        while_iterator(const iterable_t& it_, const predicate_t<>& pred_)
            : it(it_->iter()), pred(pred_), end(memnew(ItEnd)) {}
        
        inline Variant next() override {
            return pred() ? it->next() : end;
        }

        inline bool has_next() override {
            return pred() && it->has_next();
        }

    }; // END while_iterator

    while_iterable(const iterable_t& it_, const predicate_t<>& pred_)
        : it(it_), pred(pred_) {}
    
    inline iterator_t iter() override {
        return std::make_shared<while_iterator>(this->it, pred);
    }

}; // END while_iterable

template <typename T>
class rx_list {
private:
    std::list<T> _list;
public:
    rx_list() {}
    rx_list(const std::list<T>& other)
        : _list(other) {}
    rx_list(const iterable_t& other) {
        auto _it = other->iter();
        while(_it->has_next()){ 
            _list.push_back(_it->next());
        }
    }
    template<typename IterableT>
    rx_list(const IterableT& other)
        : _list(other.begin(), other.end()) {}
    ~rx_list(){}

    void push_back(const T& value) {
        _list.push_back(value);
    }
    void pop_back() {
        if (!_list.empty()) {
            _list.pop_back();
        }
    }
    bool empty() const {
        return _list.empty();
    }
    size_t size() const {
        return _list.size();
    }
    void clear() {
        _list.clear();
    }
    typename std::list<T>::iterator begin() {
        return _list.begin();
    }
    typename std::list<T>::iterator end() {
        return _list.end();
    }
    typename std::list<T>::const_iterator begin() const {
        return _list.begin();
    }
    typename std::list<T>::const_iterator end() const {
        return _list.end();
    }
}; // END class rx_list

template <typename T>
class rx_vector {
private:
    std::vector<T> _vector;
public:
    rx_vector() {}
    rx_vector(const std::vector<T>& other)
        : _vector(other) {}
    rx_vector(const iterable_t& other) {
        auto _it = other->iter();
        while(_it->has_next()){ 
            _vector.push_back(_it->next());
        }
    }
    template<typename IterableT>
    rx_vector(const IterableT& other)
        : _vector(other.begin(), other.end()) {}
    ~rx_vector() {}

    void push_back(const T& value) {
        _vector.push_back(value);
    }
    void pop_back() {
        if (!_vector.empty()) {
            _vector.pop_back();
        }
    }
    bool empty() const {
        return _vector.empty();
    }
    size_t size() const {
        return _vector.size();
    }
    void clear() {
        _vector.clear();
    }
    T& operator[](size_t index) {
        return _vector[index];
    }
    const T& operator[](size_t index) const {
        return _vector[index];
    }
    typename std::vector<T>::iterator begin() {
        return _vector.begin();
    }
    typename std::vector<T>::iterator end() {
        return _vector.end();
    }
    typename std::vector<T>::const_iterator begin() const {
        return _vector.begin();
    }
    typename std::vector<T>::const_iterator end() const {
        return _vector.end();
    }
}; // END class rx_vector

namespace iterator {

static Ref<RxIterableBase> to_iterable(const Variant& it) {
    if (auto iterable = DYN_CAST_OR_NULL(it, RxIterableBase)) {
        return iterable;
    }
    if (it.get_type() == Variant::ARRAY) {
        return RxIterableBase::wrap(std::make_shared<array_iterable>(it));
    }
    if (it.get_type() == Variant::DICTIONARY) {
        return RxIterableBase::wrap(std::make_shared<dictionary_iterable>(it));
    }
    return to_iterable(Array::make(it));
}

static Ref<RxIterableBase> iter(const Variant& it) {
    if (auto iterable = DYN_CAST_OR_NULL(it, RxIterableBase)) {
        return iterable->iter();
    }
    if (it.get_type() == Variant::ARRAY) {
        return RxIteratorBase::wrap(std::make_shared<array_iterator>(it));
    }
    if (it.get_type() == Variant::DICTIONARY) {
        return RxIteratorBase::wrap(std::make_shared<dictionary_iterator>(it));
    }
    return iter(Array::make(it));
}

} // END namespace iterator 

} // END namespace rx

#endif // RX_ITERATOR_H