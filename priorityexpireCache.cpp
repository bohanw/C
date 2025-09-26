/*
Tesla Energy software software phone screen
You can use any language.

Implement a cache which includes the following features:
  Expire Time - after which an entry in the cache is invalid
  Priority - lower priority entries should be evicted before higher priority entries
  LRU - within a given priority, the least recently used item should be evicted first

The cache should support these operations:
  Get: Get the value of the key if the key exists in the cache and is not expired.
  Set: Update or insert the value of the key with a priority value and expire time.
       This should never ever allow more items than maxItems to be in the cache.

The cache eviction strategy should be as follows:
  1. Evict an expired entry first.
  2. If there are no expired items to evict, evict the lowest priority entry.
  3. If there are multiple items with the same priority, evict the least
     recently used among them.

This datastructure is expected to hold large datasets (>>10^6 items) and should be
as efficient as possible.
*/

#include <iostream>
#include <chrono>
#include <thread>

int g_Time = 0;

// TODO - implement this
class PriorityExpiryCache {
private:
  int maxItems;
  std::unorder_map<
public:
  int* Get(std::string key) {
    return nullptr;
  }
  
  void Set(std::string key, int value, int priority, int expiryInSecs) {
    EvictItems();
  }
  
  void SetMaxItems(int numItems) {
    maxItems = numItems;
    EvictItems();
  }
  
  void DebugPrintKeys() {
  }
  
  void EvictItems() {
  };

};

int main() {

  PriorityExpiryCache c(5);
  c.Set("A", 1, 5,  100 );
  c.Set("B", 2, 15, 3   );
  c.Set("C", 3, 5,  10  );
  c.Set("D", 4, 1,  15  );
  c.Set("E", 5, 5,  150 );
  c.Get("C");


  // Current time = 0
  c.SetMaxItems(5);
  // Keys in C = ["A", "B", "C", "D", "E"]
  // space for 5 keys, all 5 items are included
  c.DebugPrintKeys();

  // Sleep for 5 secs
  g_Time += 5;

  // Current time = 5
  c.SetMaxItems(4);
  // Keys in C = ["A", "C", "D", "E"]
  // "B" is removed because it is expired.  expiryInSecs 3 < 5
  c.DebugPrintKeys();

  c.SetMaxItems(3);
  // Keys in C = ["A", "C", "E"]
  // "D" is removed because it the lowest priority
  // D's expire time is irrelevant.
  c.DebugPrintKeys();

  c.SetMaxItems(2);
  // Keys in C = ["C", "E"]
  // "A" is removed because it is least recently used."
  // A's expire time is irrelevant.
  c.DebugPrintKeys();

  c.SetMaxItems(1);
  // Keys in C = ["C"]
  // "E" is removed because C is more recently used (due to the Get("C") event).
  c.DebugPrintKeys();
  
  return 0;
}