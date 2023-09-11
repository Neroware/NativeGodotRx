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
class RxList {
private:
    std::shared_ptr<std::list<T>> list_ptr;
public:
    // Constructors
    RxList() 
        : list_ptr(std::make_shared<std::list<T>>()) {}
    RxList(const std::initializer_list<T>& init_lst) 
        : list_ptr(std::make_shared<std::list<T>>(init_lst)) {}
    RxList(const std::list<T>& other)
        : list_ptr(std::make_shared<std::list<T>>(other)) {}
    RxList(const iterable_t& other)
        : list_ptr(std::make_shared<std::list<T>>()) {
            auto _it = other->iter();
            while(_it->has_next()){ 
                list_ptr->push_back(_it->next());
            }
    }
    template<typename IterableT>
    RxList(const IterableT& other)
        : list_ptr(std::make_shared<std::list<T>>(other.begin(), other.end())) {}
    /* template<typename... Args>
    RxList(const Args&... args)
        : _list({args...}) {} */

    // Element access
    T& front() {
        return list_ptr->front();
    }
    const T& front() const {
        return list_ptr->front();
    }
    T& back() {
        return list_ptr->back();
    }
    const T& back() const {
        return list_ptr->back();
    }

    // Capacity
    bool empty() const {
        return list_ptr->empty();
    }
    size_t size() const {
        return list_ptr->size();
    }

    // Modifiers
    void clear() {
        list_ptr->clear();
    }
    void insert(typename std::list<T>::iterator pos, const T& value) {
        list_ptr->insert(pos, value);
    }
    void insert(typename std::list<T>::iterator pos, T&& value) {
        list_ptr->insert(pos, std::move(value));
    }
    typename std::list<T>::iterator erase(typename std::list<T>::iterator pos) {
        return list_ptr->erase(pos);
    }
    void push_back(const T& value) {
        list_ptr->push_back(value);
    }
    void push_back(T&& value) {
        list_ptr->push_back(std::move(value));
    }
    void pop_back() {
        list_ptr->pop_back();
    }
    void push_front(const T& value) {
        list_ptr->push_front(value);
    }
    void push_front(T&& value) {
        list_ptr->push_front(std::move(value));
    }
    void pop_front() {
        list_ptr->pop_front();
    }
    void resize(size_t new_size) {
        list_ptr->resize(new_size);
    }
    void swap(RxList& other) {
        list_ptr->swap(*(other.list_ptr));
    }

    class iterator {
    public:
        using iterator_category = typename std::list<T>::iterator::iterator_category;
        using value_type = typename std::list<T>::iterator::value_type;
        using difference_type = typename std::list<T>::iterator::difference_type;
        using pointer = typename std::list<T>::iterator::pointer;
        using reference = typename std::list<T>::iterator::reference;

        iterator(typename std::list<T>::iterator it) : internal_iterator(it) {}

        iterator& operator++() {
            ++internal_iterator;
            return *this;
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++internal_iterator;
            return temp;
        }
        bool operator==(const iterator& other) const {
            return internal_iterator == other.internal_iterator;
        }
        bool operator!=(const iterator& other) const {
            return internal_iterator != other.internal_iterator;
        }
        reference operator*() {
            return *internal_iterator;
        }
        pointer operator->() {
            return &(*internal_iterator);
        }

    private:
        typename std::list<T>::iterator internal_iterator;
    }; // END iterator

    class const_iterator {
    public:
        using iterator_category = typename std::list<T>::const_iterator::iterator_category;
        using value_type = typename std::list<T>::const_iterator::value_type;
        using difference_type = typename std::list<T>::const_iterator::difference_type;
        using pointer = typename std::list<T>::const_iterator::pointer;
        using reference = typename std::list<T>::const_iterator::reference;

        const_iterator(typename std::list<T>::const_iterator it) : internal_iterator(it) {}

        const_iterator& operator++() {
            ++internal_iterator;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++internal_iterator;
            return temp;
        }
        bool operator==(const const_iterator& other) const {
            return internal_iterator == other.internal_iterator;
        }
        bool operator!=(const const_iterator& other) const {
            return internal_iterator != other.internal_iterator;
        }
        reference operator*() {
            return *internal_iterator;
        }
        pointer operator->() {
            return &(*internal_iterator);
        }

    private:
        typename std::list<T>::const_iterator internal_iterator;
    }; // END const_iterator

    iterator begin() {
        return iterator(list_ptr->begin());
    }
    iterator end() {
        return iterator(list_ptr->end());
    }
    const_iterator cbegin() const {
        return const_iterator(list_ptr->cbegin());
    }
    const_iterator cend() const {
        return const_iterator(list_ptr->cend());
    }
    const_iterator begin() const {
        return const_iterator(list_ptr->cbegin());
    }
    const_iterator end() const {
        return const_iterator(list_ptr->cend());
    }
};

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