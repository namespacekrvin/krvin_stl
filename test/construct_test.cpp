#include "../src/construct.h"

#include <iostream>
#include <string>
#include <vector>

using namespace KrvinSTL;

struct MyClass {
    int x;

    MyClass(int v = 0) : x(v) {
        std::cout << "MyClass construct: " << x << "\n";
    }

    ~MyClass() {
        std::cout << "MyClass destroy: " << x << "\n";
    }
};

int main() {
  std::cout << "===== 1. 测试 construct 和 destroy(T*) =====\n";

  // 1. 手动分配一块裸内存，足够放一个 MyClass
  void *raw = operator new(sizeof(MyClass));

  // 在裸内存上构造对象
  construct(static_cast<MyClass*>(raw), 100);

  // 访问对象
  MyClass *p = static_cast<MyClass *>(raw);
  std::cout << "p->x = " << p->x << "\n";

  // 析构对象，但不释放内存
  KrvinSTL::destroy(p);

  // 释放裸内存
  operator delete(raw);

  std::cout << "\n===== 2. 测试 destroy(first, last) 对 POD 类型 =====\n";

  int arr[5] = {1, 2, 3, 4, 5};

  // int 是 POD 类型，理论上不会调用元素析构
  KrvinSTL::destroy(arr, arr + 5);

  std::cout << "POD 区间析构完成\n";

  std::cout << "\n===== 3. 测试 destroy(first, last) 对非 POD 类型 =====\n";

  MyClass* p1 = static_cast<MyClass*>(operator new(sizeof(MyClass) * 3));

  construct(p1 + 0, 10);
  construct(p1 + 1, 20);
  construct(p1 + 2, 30);

  KrvinSTL::destroy(p1, p1 + 3);

  operator delete(p1);

  std::cout << "非 POD 区间析构完成\n";

  return 0;
}