#ifndef RX_WEAKKEYDICTIONARY_H
#define RX_WEAKKEYDICTIONARY_H

#include <godot_cpp/variant/utility_functions.hpp>

#include <unordered_map>
#include <memory>
#include <mutex>
#include <shared_mutex>

namespace rx {

struct variant_key_t {
    godot::Variant ref;
    variant_key_t(const godot::Variant& ref_) 
        : ref(ref_) {}
    
    bool operator==(const variant_key_t& other) const {
        return this->ref == other.ref;
    }
};

}; // END namespace rx

// Specialization of std::hash for variant_key_t
namespace std {

template <>
struct hash<rx::variant_key_t> {
    size_t operator()(const rx::variant_key_t& vk) const {
        return godot::UtilityFunctions::hash(vk.ref);
    }
};

} // END namespace std

namespace rx {

template <typename Key, typename Value, int N = 10>
class weakkey_dictionary {
public:
    inline void insert(const std::shared_ptr<Key>& key, const Value& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        dictionary_[key] = value;
        insertionCounter_++;

        if (insertionCounter_ >= N) {
            this->_garbage_collect();
            insertionCounter_ = 0;
        }
    }

    inline bool contains(const std::shared_ptr<Key>& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return dictionary_.find(key) != dictionary_.end();
    }

    inline Value& operator[](const std::shared_ptr<Key>& key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        return dictionary_[key];
    }

    inline const Value& operator[](const std::shared_ptr<Key>& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return dictionary_.at(key);
    }

    inline void erase(const std::shared_ptr<Key>& key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        dictionary_.erase(key);
    }

private:
    inline void _garbage_collect() {
        std::unordered_map<std::weak_ptr<Key>, Value, weakkey_dictionary::hash, weakkey_dictionary::pred> newDictionary;
        for (const auto& entry : dictionary_) {
            if (!entry.first.expired()) {
                newDictionary[entry.first] = entry.second;
            }
        }
        dictionary_ = std::move(newDictionary);
    }

    struct hash {
        size_t operator()(const std::weak_ptr<Key>& wp) const {
            if (auto sp = wp.lock()) {
                return std::hash<Key>{}(*sp);
            } else {
                return 0;
            }
        }
    };

    struct pred {
        bool operator()(const std::weak_ptr<Key>& lhs, const std::weak_ptr<Key>& rhs) const {
            auto _lhs = lhs.lock();
            auto _rhs = rhs.lock();
            return _lhs && _rhs ? *_lhs == *_rhs : false;
        }
    };

private:
    std::unordered_map<std::weak_ptr<Key>, Value, weakkey_dictionary::hash, weakkey_dictionary::pred> dictionary_;
    int insertionCounter_ = 0;
    mutable std::shared_mutex mutex_;
};

} // END namespace rx

#endif // RX_WEAKKEYDICTIONARY_H