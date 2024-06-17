#pragma once

#include "cmath"
#include "vector"
#include "thread"
#include "future"

template<typename Type>
class DiagonalMatrix {
public:
    explicit DiagonalMatrix(size_t size) : pDataArray{new Type[(size * size - size) / 2]},
                                        nSize{size} {};
    DiagonalMatrix(DiagonalMatrix<Type>& other) = delete;
    DiagonalMatrix(DiagonalMatrix<Type>&& other) = delete;
    Type* get(size_t i, size_t j) {
        if(i == 0 and j == 0) {
            return nullptr;
        }
        if(j < i) {
            std::swap(i, j);
        }
        size_t flatIdx = (nSize * (nSize - 1) / 2) - (nSize - i) * ((nSize - i) - 1) / 2 + j - i - 1;
        return &pDataArray[flatIdx];
    }
    Type* get(size_t flatIndex) {
        if(flatIndex > size()) {
            return nullptr;
        }
        return &pDataArray[flatIndex];
    }
    void set(size_t i, size_t j, Type* value) {
        if(i == 0 and j == 0) {
            return;
        }
        if(j < i) {
            std::swap(i, j);
        }
        size_t flatIdx = (nSize * (nSize - 1) / 2) - (nSize - i) * ((nSize - i) - 1) / 2 + j - i - 1;
        pDataArray[flatIdx] = *value;
    }
    [[nodiscard]]
    size_t size() const {
        return (nSize * nSize - nSize) / 2;
    }
    Type* getRawMatrix() {
        return pDataArray;
    }
    void getSquareIndex(size_t idx, size_t& i, size_t& j) {
        i = nSize - 2 - static_cast<int>(std::floor(std::sqrt(-8 * idx + 4 * nSize * (nSize - 1) - 7) / 2.0 - 0.5));
        j = int(static_cast<double>(idx) + i + 1 - nSize * (nSize - 1) / 2.0 + (nSize - i) * ((nSize - i) - 1) / 2.0);
    }
    Type* begin() {
        return pDataArray;
    }
    Type* end() {
        return pDataArray + nSize;
    }
    ~DiagonalMatrix() {
        delete[] pDataArray;
    }
private:
    Type* pDataArray;
    size_t nSize{};
};

template<typename RAIter, typename Function>
void parallel_foreach(RAIter first, RAIter last, Function func) {
    auto distance = std::distance(first, last);
    auto elemsPerThread = 100;
    auto threadCount = std::min(int(std::thread::hardware_concurrency()), int(distance / elemsPerThread));
    auto threads = std::vector<std::thread>(threadCount);
    auto bounds = std::vector<std::pair<int, int>>(std::ceil(distance / elemsPerThread));
    for(auto i = 0; i < bounds.size(); i++) {
        auto start = i * elemsPerThread;
        auto end = start + elemsPerThread;
        bounds[i] = std::pair<int, int>(start, end);
    }
    // One solution, pretty bad if you would think about
    for(auto i = 0; i < threadCount; i++) {
        threads[i] = std::thread([&bounds, &func, i, &threadCount, &first](){
            auto boundNumber = i;
            while (boundNumber < bounds.size()) {
                auto bound = bounds[boundNumber];
                auto start = bound.first;
                auto end = bound.second;
                for(int j = start; j < end; j++) {
                    auto item = first + j;
                    func(*item);
                }
                boundNumber += threadCount;
            }
        });
    }
    for(auto i = 0; i < threadCount; i++) {
        threads[i].join();
    }
}

template<typename IdxType, typename Function>
std::enable_if<std::is_arithmetic_v<IdxType>, void>::type
parallel_for(IdxType first, IdxType last, Function func, int elemsPerThread = 100) {
    auto distance = last - first;
    auto threadCount = std::min(int(std::thread::hardware_concurrency()) - 1, int(std::ceil(distance / elemsPerThread)));
    auto threads = std::vector<std::thread>(threadCount);
    auto bounds = std::vector<std::pair<int, int>>(std::ceil(distance / elemsPerThread));
    for(auto i = 0; i < bounds.size(); i++) {
        auto start = first + i * elemsPerThread;
        auto end = start + elemsPerThread;
        bounds[i] = std::pair<int, int>(start, end);
    }
    // One solution, pretty bad if you would think about
    for(auto i = 0; i < threadCount; i++) {
        threads[i] = std::thread([&bounds, &func, i, &threadCount](){
            auto boundNumber = i;
            while (boundNumber < bounds.size()) {
                auto bound = bounds[boundNumber];
                auto start = bound.first;
                auto end = bound.second;
                for(IdxType j = start; j < end; j++) {
                    func(j);
                }
                boundNumber += threadCount;
            }
        });
    }
    auto start = first + bounds.size() * elemsPerThread;
    auto end = start + (last - start);
    for(IdxType j = start; j < end; j++) {
        func(j);
    }
    for(auto i = 0; i < threadCount; i++) {
        threads[i].join();
    }
}

static inline int some_kind_of_id = 0;

namespace __util__ {
    template<typename T, typename Comp>
    T middle_of_three(T& a, T&b, T& c, Comp comparator) {
        if(comparator(a, b) < 0) {
            if(comparator(b, c) < 0) {
                return b;
            } else if(comparator(a, c) < 0) {
                return c;
            } else {
                return a;
            }
        } else {
            if(comparator(a, c) < 0) {
                return a;
            } else if(comparator(b, c) < 0) {
                return c;
            } else {
                return b;
            }
        }
    }

    template<typename T, typename Comp>
    T get_pivot(std::vector<T>&v, int left, int right, Comp comparator) {
        return middle_of_three(v[left], v[(left + right) / 2], v[right], comparator);
    }

    template<typename T, typename Comp>
    void partition(std::vector<T>&v, T& pivot, int &i, int &j, Comp comparator) {
        while (i <= j) {
            while (comparator(v[i], pivot) < 0) {
                i++;
            }
            while (comparator(v[j], pivot) > 0) {
                j--;
            }
            if (i <= j) {
                std::swap(v[i], v[j]);
                i++;
                j--;
            }
        }
    }

    template<typename T, typename Comp, int sync_size = 10'000>
    void qsort_async_util(std::vector<T>& v, int left, int right, Comp comparator) {
        int i = left;
        int j = right;

        T pivot = get_pivot(v, left, right, comparator);
        partition(v, pivot, i, j, comparator);

        std::future<void> left_future;
        std::future<void> right_future;

        if(left < j) {
            if(j - left < sync_size) {
                qsort_async_util(v, left, j, comparator);
            } else {
                left_future = std::async(std::launch::async, [&v, left, j, comparator]() { qsort_async_util(v, left, j, comparator); });
            }
        }
        if(i < right) {
            if(right - i < sync_size) {
                qsort_async_util(v, i, right, comparator);
            } else {
                right_future = std::async(std::launch::async, [&v, i, right, comparator]() { qsort_async_util(v, i, right, comparator); });
            }
        }

        if(left_future.valid()) {
            left_future.wait();
        }
        if(right_future.valid()) {
            right_future.wait();
        }
    }
}

template<typename T, typename Comp, int sync_size = 10'000>
void qsort_async(std::vector<T>& v, int length, Comp comparator) {
    __util__::qsort_async_util<T, Comp, sync_size>(v, 0, length - 1, comparator);
}