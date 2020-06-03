// RUN: %clang_cc1 -I %S/Inputs -fsycl -fsycl-is-device -triple spir64-unknown-unknown-sycldevice -fsycl-int-header=%t.h %s -fsyntax-only
// RUN: FileCheck -input-file=%t.h %s

#include "sycl.hpp"

enum unscoped_enum : int {
  val_1,
  val_2
};

enum class no_namespace_int : int {
  val_1,
  val_2
};

enum class no_namespace_short : short {
  val_1,
  val_2
};

namespace internal {
enum class namespace_short : short {
  val_1,
  val_2
};
}

namespace {
enum class enum_in_anonNS : short {
  val_1,
  val_2
};
}

enum class no_type_set {
  val_1,
  val_2
};

template <no_namespace_int EnumType>
class dummy_functor_1 {
public:
  void operator()() {}
};

template <no_namespace_short EnumType>
class dummy_functor_2 {
public:
  void operator()() {}
};

template <internal::namespace_short EnumType>
class dummy_functor_3 {
public:
  void operator()() {}
};

template <enum_in_anonNS EnumType>
class dummy_functor_4 {
public:
  void operator()() {}
};

template <no_type_set EnumType>
class dummy_functor_5 {
public:
  void operator()() {}
};

template <unscoped_enum EnumType>
class dummy_functor_6 {
public:
  void operator()() {}
};

template <typename EnumType>
class dummy_functor_7 {
public:
  void operator()() {}
};

namespace type_argument_template_enum {
enum class E : int {
  A,
  B,
  C
};
}

template <typename T>
class T1 {};
template <type_argument_template_enum::E EnumValue>
class T2 {};
template <typename EnumType>
class T3 {};

int main() {

  dummy_functor_1<no_namespace_int::val_1> f1;
  dummy_functor_2<no_namespace_short::val_2> f2;
  dummy_functor_3<internal::namespace_short::val_2> f3;
  dummy_functor_4<enum_in_anonNS::val_2> f4;
  dummy_functor_5<no_type_set::val_1> f5;
  dummy_functor_6<unscoped_enum::val_1> f6;
  dummy_functor_7<no_namespace_int> f7;
  dummy_functor_7<internal::namespace_short> f8;

  cl::sycl::queue q;

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f1);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f2);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f3);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f4);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f5);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f6);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f7);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task(f8);
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task<T1<T2<type_argument_template_enum::E::A>>>([=]() {});
  });

  q.submit([&](cl::sycl::handler &cgh) {
    cgh.single_task<T1<T3<type_argument_template_enum::E>>>([=]() {});
  });

  return 0;
}

// CHECK: Forward declarations of templated kernel function types:
// CHECK: enum class no_namespace_int : int;
// CHECK: template <no_namespace_int EnumType> class dummy_functor_1;
// CHECK: enum class no_namespace_short : short;
// CHECK: template <no_namespace_short EnumType> class dummy_functor_2;
// CHECK: namespace internal {
// CHECK-NEXT: enum class namespace_short : short;
// CHECK-NEXT: }
// CHECK: template <internal::namespace_short EnumType> class dummy_functor_3;
// CHECK: namespace  {
// CHECK-NEXT: enum class enum_in_anonNS : short;
// CHECK-NEXT: }
// CHECK: template <enum_in_anonNS EnumType> class dummy_functor_4;
// CHECK: enum class no_type_set : int;
// CHECK: template <no_type_set EnumType> class dummy_functor_5;
// CHECK: enum unscoped_enum : int;
// CHECK: template <unscoped_enum EnumType> class dummy_functor_6;
// CHECK: template <typename EnumType> class dummy_functor_7;
// CHECK: namespace type_argument_template_enum {
// CHECK-NEXT: enum class E : int;
// CHECK-NEXT: }
// CHECK: template <type_argument_template_enum::E EnumValue> class T2;
// CHECK: template <typename T> class T1;
// CHECK: Specializations of KernelInfo for kernel function types:
// CHECK: template <> struct KernelInfo<::dummy_functor_1<(no_namespace_int)0>>
// CHECK: template <> struct KernelInfo<::dummy_functor_2<(no_namespace_short)1>>
// CHECK: template <> struct KernelInfo<::dummy_functor_3<(internal::namespace_short)1>>
// CHECK: template <> struct KernelInfo<::dummy_functor_4<(enum_in_anonNS)1>>
// CHECK: template <> struct KernelInfo<::dummy_functor_5<(no_type_set)0>>
// CHECK: template <> struct KernelInfo<::dummy_functor_6<(unscoped_enum)0>>
// CHECK: template <> struct KernelInfo<::dummy_functor_7<::no_namespace_int>>
// CHECK: template <> struct KernelInfo<::dummy_functor_7<::internal::namespace_short>>
// CHECK: template <> struct KernelInfo<::T1<::T2<(type_argument_template_enum::E)0>>>
// CHECK: template <> struct KernelInfo<::T1<::T3<::type_argument_template_enum::E>>>
