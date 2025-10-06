#include <graphs.hpp>

int main() {
    Graph adj_list(10);
    adj_list.add_edge(1, 2);
    adj_list.add_edge(2, 3);

    adj_list.print();
}