// RUN: mlir-opt %s | mlir-opt | FileCheck %s

module {
  llvm.module_flags [#llvm.mlir.module_flag<error, "wchar_size", 4 : i32>,
                     #llvm.mlir.module_flag<min, "PIC Level", 2 : i32>,
                     #llvm.mlir.module_flag<max, "PIE Level", 2 : i32>,
                     #llvm.mlir.module_flag<max, "uwtable", 2 : i32>,
                     #llvm.mlir.module_flag<max, "frame-pointer", 1 : i32>,
                     #llvm.mlir.module_flag<override, "probe-stack", "inline-asm">]
}

// CHECK: llvm.module_flags [
// CHECK-SAME: #llvm.mlir.module_flag<error, "wchar_size", 4 : i32>,
// CHECK-SAME: #llvm.mlir.module_flag<min, "PIC Level", 2 : i32>,
// CHECK-SAME: #llvm.mlir.module_flag<max, "PIE Level", 2 : i32>,
// CHECK-SAME: #llvm.mlir.module_flag<max, "uwtable", 2 : i32>,
// CHECK-SAME: #llvm.mlir.module_flag<max, "frame-pointer", 1 : i32>,
// CHECK-SAME: #llvm.mlir.module_flag<override, "probe-stack", "inline-asm">]
