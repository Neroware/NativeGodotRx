#ifndef RX_INTERNAL_MAPPING_H
#define RX_INTERNAL_MAPPING_H

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <unordered_map>

#include "exception/exception.h"

using namespace godot;

namespace std {
    template<>
    struct hash<Variant> {
        size_t operator()(const Variant& obj) const {
            return UtilityFunctions::hash(obj);
        }
    };
} // END namespace std

namespace rx {

template <typename KeyT, typename ElemT>
struct default_mapping {

    std::unordered_map<KeyT, ElemT> mapping;

    default_mapping(const std::unordered_map<KeyT, ElemT>& mapping_) 
        : mapping(mapping_) {}
    default_mapping(const Dictionary& dict_) {
        Array keys_ = dict_.keys();
        for (auto i = 0ul; i < keys_.size(); i++) {
            KeyT key = keys_[i];
            ElemT elem = dict_[key];
            mapping[key] = elem;
        }
    }
    template<typename MappingT>
    default_mapping(const MappingT& mapping_) {
        for (const auto& pair : mapping_) {
            KeyT key = pair.first;
            ElemT elem = pair.second;
            mapping[key] = elem;
        }
    }

    inline ElemT operator[](const KeyT& key) const {
        return mapping.at(key);
    }

    inline bool contains(const KeyT& key) const {
        return mapping.find(key) != mapping.end();
    }
};

} // END namespace rx

#endif // RX_INTERNAL_MAPPING_H