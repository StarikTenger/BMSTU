#include <iostream>
#include <vector>
#include <algorithm>

class Sequence {
public:
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = int*;
        using reference = int&;

        Iterator(int pos, std::vector<std::pair<int, int>>* data) : pos(pos), data(data) {}

        reference operator*() const { return (*data)[pos].second; }
        pointer operator->() { return &(*data)[pos].second; }
        Iterator& operator++() { pos++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.pos == b.pos; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.pos != b.pos; };

    private:
        int pos = 0;
        std::vector<std::pair<int, int>>* data;
    };
    
    Iterator begin() { return Iterator(0, &data); }
    Iterator end() { return Iterator(data.size(), &data); }

    Sequence() {}
    Sequence(std::vector<int> _data) {
        data = {};
        std::for_each(_data.begin(), _data.end(), [&](int n) {data.push_back({ count_bits(n), n }); });
        std::sort(data.begin(), data.end());
    }
private:
    std::vector<std::pair<int, int>> data;
    int count_bits(int n) {
        int count = 0;
        for (int i = 0; i < sizeof(int) * 4; i++)
            count += (n >> i) & 1;
        return count;
    }
};

int main() {
    Sequence seq({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

    for (auto i : seq) {
        std::cout << i << "\n";
    }
}