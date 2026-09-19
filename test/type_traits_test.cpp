#include "../src/type_traits.h"

#include <iostream>
#include <cstring>
#include <new>

namespace KrvinSTL {
  template<typename Input, typename Output> 
  Output uninitialized_copy_impl(Input first, Input last, Output dest, _true_type) {
    std::cout << "Use memcpy for pod type!" << std::endl;
    std::memcpy(dest, first, (last - first) * sizeof(*first));
    return dest + (last - first);
  }

  template<typename Input, typename Output> 
  Output uninitialized_copy_impl(Input first, Input last, Output dest, _false_type) {
    std::cout << "Use copy construct for unpod type!" << std::endl;
    Output current = dest;
    for (; first != last; ++first, ++current) {
      new (static_cast<void*>(&(*current))) typename std::remove_reference<decltype(*first)>::type(*first);
    }
    return current;
  }

  template<typename Input, typename Output>
  Output uninitialized_copy(Input first, Input last, Output dest) {
    using value_type = typename std::remove_reference<decltype(*first)>::type;
    using is_pod = typename _type_traits<value_type>::is_POD_type;

    return uninitialized_copy_impl(first, last, dest, is_pod());
  }
}

struct ComplexObj {
  int *data;
  ComplexObj() : data(new int(1)) {}
  ComplexObj(const ComplexObj& obj) : data(new int(*obj.data)) {
    std::cout << "ComplextObj call copy construct!" << std::endl;
  }
  ~ComplexObj() { delete data;}
};

int main() {
  // 测试1：POD 类型（int 数组）
  std::cout << "=== 测试 POD 类型 ===\n";
  int src_int[] = {1, 2, 3, 4, 5};
  int dest_int[5];
  KrvinSTL::uninitialized_copy(src_int, src_int + 5, dest_int);
  for (int v : dest_int) std::cout << v << " ";
  std::cout << "\n\n";

  // 测试2：非 POD 类型（自定义类数组）
  std::cout << "=== 测试非 POD 类型 ===\n";
  ComplexObj src_obj[3];
  alignas(ComplexObj) char buffer[3 * sizeof(ComplexObj)];  // 裸内存
  ComplexObj* dest_obj = reinterpret_cast<ComplexObj*>(buffer);
  KrvinSTL::uninitialized_copy(src_obj, src_obj + 3, dest_obj);
  std::cout << "拷贝完成，dest_obj[0].data = " << *dest_obj[0].data << "\n";

  return 0;
}
