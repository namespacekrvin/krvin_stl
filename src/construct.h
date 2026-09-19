#ifndef _KRVIN_CONSTRUCT_H_
#define _KRVIN_CONSTRUCT_H_

#include "type_traits.h"

#include <new>
#include <iterator>

namespace KrvinSTL {

  template<class Ta, class Tb>
  inline void construct(Ta *ptr, const Tb &value) {
    new(ptr) Ta(value);
  }

  template<class T>
  inline void destroy(T *ptr) {
    ptr->~T();
  }

  template<class ForwardIterator>
  inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type) {}

  template<class ForwardIterator>
  inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type) {
    for (; first != last; ++first) {
      destroy(&(*first));
    }
  }

  // template<class ForwardIterator>
  // inline void destroy(ForwardIterator first, ForwardIterator last) {
  //   // typedef typename _type_traits<ForwardIterator>::is_POD_type is_pod_type;
  //   using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
  //   using is_pod_type = typename _type_traits<value_type>::is_POD_type;
  //   _destroy(first, last, is_pod_type());
  // }

  template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last){
    using is_pod_type = typename _type_traits<ForwardIterator>::is_POD_type;
		_destroy(first, last, is_pod_type());
	}
}

#endif