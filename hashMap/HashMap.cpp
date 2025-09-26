#include <iostream>
#include <list>
#include <optional>
#include <vector>
template <typename K, typename V>
class HashMap{

private:
    struct Entry{
        K key;
        V value;
        Entry(const K& k, const V& v): key(k), value(v) {}
    };

    std::vector<std::list<Entry> > hashTable;

    size_t capacity;
    size_t size;

    size_t hash(const K& key) const{
        return std::hash<K>{}(key) % capacity;
    }
    void rehash()
    {
        if(static_cast<float>(size) / static_cast<float>(capacity) > 0.75){
            size_t old_capacity = capacity;
            capacity *= 2;
            std::vector<std::list<Entry> > newTable(capacity);

            for (size_t i = 0; i < old_capacity; ++i){
                for (const auto& entry : hashTable[i]){
                    size_t idx = std::hash<K>{}(entry.key) % capacity;
                    newTable[idx].emplace_back(entry.key, entry.value);
                }
            }
            hashTable = std::move(newTable);
        }
    }

    public:
    HashMap(size_t cap = 4): capacity(cap), size(0){
        hashTable.resize(capacity);
    }

    void insert(const K& key, const V& val){
        rehash(); // rehash if load factor exceeds threshold
        size_t idx = hash(key);
        for (auto& entry : hashTable[idx]){
            if (entry.key == key){
                entry.value = val;
                return;
            }
        }
        hashTable[idx].emplace_back(key, val);
        size++;
    }

    std::optional<V> get(const K& key) const{
        size_t idx = hash(key);
        for (const auto& entry : hashTable[idx]){
            if (entry.key == key){
                return entry.value;
            }
        }
        return std::nullopt;
    }

    bool remove( const K& key){
        auto& bucket = hashTable[hash(key)];

        for(auto it = bucket.begin();it != bucket.end();++it){
            if (it -> key == key){
                bucket.erase(it);
                size--;
                return true;
            }
        }

        return false;
    }

    size_t get_size() const {return size;}

    bool contains(const K& key) const {
        size_t idx = hash(key);
        for (const auto& entry : hashTable[idx]){
            if (entry.key == key){
                return true;
            }
        }
        return false;
    }
};

// Usage example
int main() {
    HashMap<int, std::string> map;
    map.insert(1, "One");
    map.insert(2, "Two");
    map.insert(3, "Three");
    map.insert(4, "Two");
    map.insert(5, "Two");

    auto result = map.get(5);
    if (result.has_value()) {
        std::cout << "Key 5 has value: " << result.value() << std::endl;
    }

    map.remove(5);
    result = map.get(5);
    if (!result.has_value()) {
        std::cout << "Key 2 has been removed." << std::endl;
    }

    return 0;
}