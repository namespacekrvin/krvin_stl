#include "../src/alloc.h"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace KrvinSTL;

// 测试1：基本分配与释放
void test_basic_alloc_dealloc() {
    std::cout << "=== test_basic_alloc_dealloc ===" << std::endl;

    void* p1 = alloc::allocate(8);
    void* p2 = alloc::allocate(16);
    void* p3 = alloc::allocate(32);

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(p3 != nullptr);

    // 验证分配的内存可写
    std::memset(p1, 'A', 8);
    std::memset(p2, 'B', 16);
    std::memset(p3, 'C', 32);

    alloc::deallocate(p1, 8);
    alloc::deallocate(p2, 16);
    alloc::deallocate(p3, 32);

    std::cout << "PASS" << std::endl;
}

// 测试2：释放后重新分配，验证自由链表复用
void test_free_list_reuse() {
    std::cout << "=== test_free_list_reuse ===" << std::endl;

    void* p1 = alloc::allocate(32);
    assert(p1 != nullptr);

    alloc::deallocate(p1, 32);

    // 再次分配同样大小，理论上会从自由链表中取出
    void* p2 = alloc::allocate(32);
    assert(p2 != nullptr);

    // 不要求 p1 == p2，但要求 p2 可用
    std::memset(p2, 'X', 32);

    alloc::deallocate(p2, 32);

    std::cout << "PASS" << std::endl;
}

// 测试3：多次分配不同大小
void test_multiple_sizes() {
    std::cout << "=== test_multiple_sizes ===" << std::endl;

    void* ptrs[16] = {};

    for (int i = 0; i < 16; ++i) {
        size_t size = static_cast<size_t>((i + 1) * 8); // 8, 16, ..., 128
        ptrs[i] = alloc::allocate(size);
        assert(ptrs[i] != nullptr);
        std::memset(ptrs[i], static_cast<char>(i), size);
    }

    for (int i = 0; i < 16; ++i) {
        size_t size = static_cast<size_t>((i + 1) * 8);
        alloc::deallocate(ptrs[i], size);
    }

    std::cout << "PASS" << std::endl;
}

// 测试4：大块内存走 malloc 路径
void test_large_block() {
    std::cout << "=== test_large_block ===" << std::endl;

    // 大于 128 字节，应走 malloc/free
    void* p1 = alloc::allocate(129);
    void* p2 = alloc::allocate(256);
    void* p3 = alloc::allocate(1024);

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(p3 != nullptr);

    std::memset(p1, 'L', 129);
    std::memset(p2, 'M', 256);
    std::memset(p3, 'H', 1024);

    alloc::deallocate(p1, 129);
    alloc::deallocate(p2, 256);
    alloc::deallocate(p3, 1024);

    std::cout << "PASS" << std::endl;
}

// 测试5：reallocate
void test_reallocate() {
    std::cout << "=== test_reallocate ===" << std::endl;

    void* p1 = alloc::allocate(32);
    assert(p1 != nullptr);
    std::memset(p1, 'R', 32);

    void* p2 = alloc::reallocate(p1, 32, 64);
    assert(p2 != nullptr);

    // 注意：你现在的 reallocate 是先释放再分配，不保证保留旧数据
    std::memset(p2, 'N', 64);

    alloc::deallocate(p2, 64);

    std::cout << "PASS" << std::endl;
}

// 测试6：边界大小测试
void test_boundary_sizes() {
    std::cout << "=== test_boundary_sizes ===" << std::endl;

    // 测试 1 字节、7 字节、8 字节、127、128、129
    size_t sizes[] = {1, 7, 8, 15, 16, 63, 64, 127, 128, 129};

    void* ptrs[sizeof(sizes) / sizeof(sizes[0])] = {};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        ptrs[i] = alloc::allocate(sizes[i]);
        assert(ptrs[i] != nullptr);
    }

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        alloc::deallocate(ptrs[i], sizes[i]);
    }

    std::cout << "PASS" << std::endl;
}

int main() {
    test_basic_alloc_dealloc();
    test_free_list_reuse();
    test_multiple_sizes();
    test_large_block();
    test_reallocate();
    test_boundary_sizes();

    std::cout << "All basic tests passed." << std::endl;
    return 0;
}