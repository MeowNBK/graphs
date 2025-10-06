#include <graphs.hpp>
#include <queue>
#include <sstream>
#include <cstdint>

using namespace std;

namespace meow{
class boolarray {
public:
    // --- Constructor ---
    explicit boolarray(size_t size, bool init_value = false) noexcept: data_(nullptr), size_(size) {
        grow(size);
        fill(init_value);
    }
    explicit boolarray(const boolarray& other) noexcept: data_(new uint64_t[other.mask_num_]), mask_num_(other.mask_num_), size_(other.size_) {
        for (size_t i = 0; i < mask_num_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    explicit boolarray(boolarray&& other) noexcept: data_(other.data_), mask_num_(other.mask_num_), size_(other.size_) {
        other.data_ = nullptr;
        other.mask_num_ = other.size_ = 0;
    }
    inline boolarray& operator=(const boolarray& other) noexcept {
        if (this == &other) return *this;
        mask_num_ = other.mask_num_;
        size_ = other.size_;
        delete[] data_;
        data_ = new uint64_t[mask_num_];
        for (size_t i = 0; i < mask_num_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }
    inline boolarray& operator=(boolarray&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;
        data_ = other.data_;
        mask_num_ = other.mask_num_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.mask_num_ = other.size_ = 0;
        return *this;
    }
    ~boolarray() noexcept { delete[] data_; }

    // --- Boolean access ---

    /// @brief Unchecked boolean access, for performance-critical code
    [[nodiscard]] inline bool get(size_t bit_number) const noexcept {
        size_t index = bit_number / 64;
        size_t bit_pos = bit_number % 64;
        return ((data_[index]) & (1ULL << bit_pos)) != 0;
    }
    /// @brief Unchecked boolean modification, for performance-critical code
    inline void set(size_t bit_number, bool value) noexcept {
        size_t index = bit_number / 64;
        size_t bit_pos = bit_number % 64;

        if (value) data_[index] |= (1ULL << bit_pos);
        else data_[index] &= ~(1ULL << bit_pos);
    }
    inline bool operator[](size_t index) const noexcept {
        return get(index);
    }

    // --- Modifiers ---
    inline void fill(bool value) noexcept {
        if (value) {
            for (size_t i = 0; i < mask_num_; ++i) {
                data_[i] = -1;
            }
        } else {
            for (size_t i = 0; i < mask_num_; ++i) {
                data_[i] = 0;
            }
        }
    }
private:
    inline void grow(size_t new_size) noexcept {
        mask_num_ = (new_size + 63) / 64;
        if (mask_num_ < 1) mask_num_ = 1;
        uint64_t* temp_data = new uint64_t[mask_num_];
        if (data_) {
            for (size_t i = 0; i < mask_num_; ++i) {
                temp_data[i] = data_[i];
            }
            delete[] data_;
        }
        data_ = temp_data;
    }

    uint64_t* data_;
    size_t mask_num_;
    size_t size_;
};
}

inline void BFS(size_t start, const Graph<size_t>& graph) noexcept {
    meow::boolarray visited(graph.get_vertices(), false);
    queue<size_t> q;
    ostringstream os;

    visited.set(start, true);
    q.push(start);

    while (!q.empty()) {
        size_t u = q.front(); q.pop();
        os << u << " ";
        for (size_t v : graph[u]) {
            if (!visited[v]) {
                visited.set(v, true);
                q.push(v);
            }
        }
    }

    cout << os.str();
}


int main() {
    Graph<size_t> graph(6);

    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 4};
    graph[3] = {1, 5};
    graph[4] = {1, 2};
    graph[5] = {3};

    BFS(0, graph);
    cout << endl;
}