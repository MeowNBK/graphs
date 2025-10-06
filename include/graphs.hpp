#pragma once

#include <type_traits>
#include <vector>
#include <iostream>
#include <sstream>

template <typename VertexType = size_t, bool UseMatrix = false>
class Graph {
private:
    // --- Graph metadata ---
    std::vector<std::vector<VertexType>> data_;
    size_t vertices_;
public:
    // --- Constructors & destructor ---
    explicit Graph(size_t vertices = 10): vertices_(vertices) {
        if constexpr (UseMatrix) {
            data_.resize(vertices_, std::vector<VertexType>(vertices_, 0));
        } else {
            data_.resize(vertices_, std::vector<VertexType>());
        }
    }
    explicit Graph(const Graph& other) noexcept: data_(other.data_), vertices_(other.vertices_) {}
    explicit Graph(Graph&& other) noexcept: data_(std::move(other.data_)), vertices_(other.vertices_) {
        other.vertices_ = 0;
    }
    inline Graph& operator=(const Graph& other) noexcept {
        if (this == &other) return *this;
        data_ = other.data_;
        vertices_ = other.vertices_;
    }
    inline Graph& operator=(Graph&& other) noexcept {
        if (this == &other) return *this;
        data_ = std::move(other.data_);
        vertices_ = other.vertices_;
        other.vertices_ = 0;
    }
    ~Graph() noexcept = default;

    // --- Modifiers ---
    inline void add_edge(VertexType u, VertexType v) noexcept {
        if constexpr (UseMatrix) {
            data_[u][v] = 1;
            data_[v][u] = 1;
        } else {
            data_[u].push_back(v);
            data_[v].push_back(u);
        }
    }
    inline void print() const noexcept {
        std::ostringstream os; // slow but not so bad
        os << "----- Printing graph -----\n";
        if constexpr (UseMatrix) {
            os << "===== Adjencency Matrix =====\n";
            for (size_t i = 0; i < vertices_; ++i) {
                for (size_t j = 0; j < vertices_; ++j) {
                    os << data_[i][j] << "  ";
                }
                os << '\n';
            }
        } else {
            os << "===== Adjencency List =====\n";
            for (size_t i = 0; i < vertices_; ++i) {
                os << "Vertex " << i << ": ";
                for (size_t j = 0; j < data_[i].size(); ++j) {
                    os << data_[i][j] << ((j == (data_[i].size() - 1)) ? "" : ", ");
                }
                os << '\n';
            }
        }
        std::cout << os.str();
    }

    // --- Element access ---
    inline const std::vector<VertexType>& operator[](size_t index) const noexcept {
        return data_[index];
    }
    inline std::vector<VertexType>& operator[](size_t index) noexcept {
        return data_[index];
    }
    [[nodiscard]] inline size_t get_vertices() const noexcept {
        return vertices_;
    }
};