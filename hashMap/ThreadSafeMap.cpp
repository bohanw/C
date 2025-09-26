#include <iostream>
#include <list>
#include <optional>
#include <vector>
#include <mutex>
#include <functional>

template <typename K, typename V>
class HashMap {
private:
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    std::vector<std::list<Entry> > hashTable; // Buckets for chaining
    std::vector<std::mutex> bucketMutexes;  // Mutex for each bucket
    size_t capacity;                         // Number of buckets
    size_t size;                             // Current number of elements
    const float loadFactorThreshold = 0.75; // Threshold for rehashing
    std::mutex rehashMutex;                  // Mutex for rehashing

    // Hash function
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    // Rehash the hash table when the load factor exceeds the threshold
    void rehash() {
        std::lock_guard<std::mutex> rehashLock(rehashMutex); // Lock the rehashing process

        if (static_cast<float>(size) / static_cast<float>(capacity) <= loadFactorThreshold) {
            return; // No need to rehash
        }

        size_t newCapacity = capacity * 2; // Double the capacity
        std::vector<std::list<Entry>> newHashTable(newCapacity);
        std::vector<std::mutex> newBucketMutexes(newCapacity);

        // Rehash all existing entries into the new table
        for (size_t i = 0; i < capacity; ++i) {
            std::lock_guard<std::mutex> bucketLock(bucketMutexes[i]); // Lock the current bucket
            for (const auto& entry : hashTable[i]) {
                size_t newIndex = std::hash<K>{}(entry.key) % newCapacity;
                newHashTable[newIndex].emplace_back(entry.key, entry.value);
            }
        }

        // Replace the old table and mutexes with the new ones
        hashTable = std::move(newHashTable);
        bucketMutexes = std::move(newBucketMutexes);
        capacity = newCapacity;
    }

public:
    // Constructor
    HashMap(size_t cap = 4) : capacity(cap), size(0) {
        hashTable.resize(capacity);
        bucketMutexes.resize(capacity); // One mutex per bucket
    }

    // Insert a key-value pair
    void insert(const K& key, const V& val) {
        size_t idx = hash(key);
        {
            std::lock_guard<std::mutex> lock(bucketMutexes[idx]); // Lock the bucket
            for (auto& entry : hashTable[idx]) {
                if (entry.key == key) {
                    entry.value = val; // Update value if key exists
                    return;
                }
            }
            hashTable[idx].emplace_back(key, val); // Insert new entry
            size++;
        }

        // Trigger rehashing if the load factor exceeds the threshold
        if ((float)size / capacity > loadFactorThreshold) {
            rehash();
        }
    }

    // Retrieve a value by key
    std::optional<V> get(const K& key) const {
        size_t idx = hash(key);
        std::lock_guard<std::mutex> lock(bucketMutexes[idx]); // Lock the bucket
        for (const auto& entry : hashTable[idx]) {
            if (entry.key == key) {
                return entry.value; // Return the value if key is found
            }
        }
        return std::nullopt; // Key not found
    }

    // Remove a key-value pair
    bool remove(const K& key) {
        size_t idx = hash(key);
        std::lock_guard<std::mutex> lock(bucketMutexes[idx]); // Lock the bucket
        auto& bucket = hashTable[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it); // Erase the entry
                size--;
                return true;
            }
        }
        return false; // Key not found
    }

    // Get the current size of the hash map
    size_t get_size() const {
        return size;
    }

    // Check if a key exists in the hash map
    bool contains(const K& key) const {
        size_t idx = hash(key);
        std::lock_guard<std::mutex> lock(bucketMutexes[idx]); // Lock the bucket
        for (const auto& entry : hashTable[idx]) {
            if (entry.key == key) {
                return true; // Key found
            }
        }
        return false; // Key not found
    }
};

int main() {
    HashMap<int, std::string> map;

    // Insert key-value pairs
    map.insert(1, "One");
    map.insert(2, "Two");
    map.insert(3, "Three");

    // Retrieve a value
    auto result = map.get(2);
    if (result.has_value()) {
        std::cout << "Key 2 has value: " << result.value() << std::endl;
    }

    // Remove a key
    map.remove(2);
    result = map.get(2);
    if (!result.has_value()) {
        std::cout << "Key 2 has been removed." << std::endl;
    }

    return 0;
}