#ifndef STORAGE_H
#define STORAGE_H

// Generic storage container using a fixed-size static array.
// std::vector is prohibited; this template replaces it.
template <typename T>
class Storage {
private:
    T   data[100];
    int count;

public:
    Storage() : count(0) {}
    ~Storage() {}

    // Add an element (returns false if full)
    bool add(const T& item) {
        if (count >= 100) return false;
        data[count++] = item;
        return true;
    }

    // Remove element at logical index (shifts remaining elements) — public
    bool removeAt(int index) {
        if (index < 0 || index >= count) return false;
        for (int i = index; i < count - 1; ++i)
            data[i] = data[i + 1];
        --count;
        return true;
    }

    // Remove by ID field — T must expose getId()
    bool removeById(int id) {
        for (int i = 0; i < count; ++i) {
            if (data[i].getId() == id) {
                return removeAt(i);
            }
        }
        return false;
    }

    // Find index by ID (-1 if not found)
    int findIndexById(int id) const {
        for (int i = 0; i < count; ++i)
            if (data[i].getId() == id) return i;
        return -1;
    }

    // Find pointer by ID (nullptr if not found)
    T* findById(int id) {
        for (int i = 0; i < count; ++i)
            if (data[i].getId() == id) return &data[i];
        return nullptr;
    }

    const T* findById(int id) const {
        for (int i = 0; i < count; ++i)
            if (data[i].getId() == id) return &data[i];
        return nullptr;
    }

    // Access by index (no bounds check — caller must ensure valid index)
    T& get(int index) { return data[index]; }
    const T& get(int index) const { return data[index]; }

    // Number of stored elements
    int size() const { return count; }

    // Reset storage
    void clear() { count = 0; }

    // Pointer to internal array (for iteration)
    T* getAll() { return data; }
    const T* getAll() const { return data; }
};

#endif

