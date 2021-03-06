// RUN: %clang_cc1 -triple i686-windows-msvc   -emit-llvm -std=c++1y -O0 -o - %s | FileCheck %s
struct Base {};

// __declspec(dllexport) causes us to export the implicit constructor.
struct __declspec(dllexport) Derived : virtual Base {
// CHECK-LABEL: define weak_odr dllexport x86_thiscallcc %"struct.\01?Derived@@"* @"\01??0Derived@@QAE@ABU0@@Z"
// CHECK:      %[[this:.*]] = load %"struct.\01?Derived@@"** {{.*}}
// CHECK-NEXT: store %"struct.\01?Derived@@"* %[[this]], %"struct.\01?Derived@@"** %[[retval:.*]]
// CHECK:      %[[dest_a_gep:.*]] = getelementptr inbounds %"struct.\01?Derived@@"* %[[this]], i32 0, i32 1
// CHECK-NEXT: %[[src_load:.*]]   = load %"struct.\01?Derived@@"** {{.*}}
// CHECK-NEXT: %[[src_a_gep:.*]]  = getelementptr inbounds %"struct.\01?Derived@@"* %[[src_load:.*]], i32 0, i32 1
// CHECK-NEXT: call void @llvm.memcpy.p0i8.p0i8.i64(i8* %[[dest_a_gep]], i8* %[[src_a_gep]], i64 1, i32 4, i1 false)
// CHECK-NEXT: %[[dest_this:.*]] = load %"struct.\01?Derived@@"** %[[retval]]
// CHECK-NEXT: ret %"struct.\01?Derived@@"* %[[dest_this]]
  bool a : 1;
  bool b : 1;
};

// __declspec(dllexport) causes us to export the implicit copy constructor.
struct __declspec(dllexport) Derived2 : virtual Base {
// CHECK-LABEL: define weak_odr dllexport x86_thiscallcc %"struct.\01?Derived2@@"* @"\01??0Derived2@@QAE@ABU0@@Z"
// CHECK:      %[[this:.*]] = load %"struct.\01?Derived2@@"** {{.*}}
// CHECK-NEXT: store %"struct.\01?Derived2@@"* %[[this]], %"struct.\01?Derived2@@"** %[[retval:.*]]
// CHECK:      %[[dest_a_gep:.*]] = getelementptr inbounds %"struct.\01?Derived2@@"* %[[this]], i32 0, i32 1
// CHECK-NEXT: %[[src_load:.*]]   = load %"struct.\01?Derived2@@"** {{.*}}
// CHECK-NEXT: %[[src_a_gep:.*]]  = getelementptr inbounds %"struct.\01?Derived2@@"* %[[src_load:.*]], i32 0, i32 1
// CHECK-NEXT: %[[dest_a_bitcast:.*]]  = bitcast [1 x i32]* %[[dest_a_gep]] to i8*
// CHECK-NEXT: %[[src_a_bitcast:.*]] = bitcast [1 x i32]* %[[src_a_gep]] to i8*
// CHECK-NEXT: call void @llvm.memcpy.p0i8.p0i8.i32(i8* %[[dest_a_bitcast]], i8* %[[src_a_bitcast]], i32 4, i32 4, i1 false)
// CHECK-NEXT: %[[dest_this:.*]] = load %"struct.\01?Derived2@@"** %[[retval]]
// CHECK-NEXT: ret %"struct.\01?Derived2@@"* %[[dest_this]]
  int Array[1];
};
