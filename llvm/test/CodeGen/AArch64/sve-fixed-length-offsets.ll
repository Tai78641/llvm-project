; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 5
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve < %s | FileCheck %s
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve -aarch64-sve-vector-bits-min=128 -aarch64-sve-vector-bits-max=128 < %s | FileCheck %s --check-prefix=CHECK-128
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve -aarch64-sve-vector-bits-min=256 -aarch64-sve-vector-bits-max=256 < %s | FileCheck %s --check-prefix=CHECK-256
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve -aarch64-sve-vector-bits-min=512 -aarch64-sve-vector-bits-max=512 < %s | FileCheck %s --check-prefix=CHECK-512
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve -aarch64-sve-vector-bits-min=1024 -aarch64-sve-vector-bits-max=1024 < %s | FileCheck %s --check-prefix=CHECK-1024
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve -aarch64-sve-vector-bits-min=2048 -aarch64-sve-vector-bits-max=2048 < %s | FileCheck %s --check-prefix=CHECK-2048

define void @nxv16i8(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv16i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b
; CHECK-NEXT:    mov w8, #256 // =0x100
; CHECK-NEXT:    ld1b { z0.b }, p0/z, [x0, x8]
; CHECK-NEXT:    st1b { z0.b }, p0, [x1, x8]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv16i8:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ldr z0, [x0, #16, mul vl]
; CHECK-128-NEXT:    str z0, [x1, #16, mul vl]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv16i8:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ldr z0, [x0, #8, mul vl]
; CHECK-256-NEXT:    str z0, [x1, #8, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv16i8:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ldr z0, [x0, #4, mul vl]
; CHECK-512-NEXT:    str z0, [x1, #4, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv16i8:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ldr z0, [x0, #2, mul vl]
; CHECK-1024-NEXT:    str z0, [x1, #2, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv16i8:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ldr z0, [x0, #1, mul vl]
; CHECK-2048-NEXT:    str z0, [x1, #1, mul vl]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i8, ptr %ldptr, i64 256
  %stoff = getelementptr inbounds nuw i8, ptr %stptr, i64 256
  %x = load <vscale x 16 x i8>, ptr %ldoff, align 1
  store <vscale x 16 x i8> %x, ptr %stoff, align 1
  ret void
}

define void @nxv8i16(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv8i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h
; CHECK-NEXT:    mov x8, #128 // =0x80
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0, x8, lsl #1]
; CHECK-NEXT:    st1h { z0.h }, p0, [x1, x8, lsl #1]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv8i16:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ldr z0, [x0, #16, mul vl]
; CHECK-128-NEXT:    str z0, [x1, #16, mul vl]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv8i16:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ldr z0, [x0, #8, mul vl]
; CHECK-256-NEXT:    str z0, [x1, #8, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv8i16:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ldr z0, [x0, #4, mul vl]
; CHECK-512-NEXT:    str z0, [x1, #4, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv8i16:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ldr z0, [x0, #2, mul vl]
; CHECK-1024-NEXT:    str z0, [x1, #2, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv8i16:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ldr z0, [x0, #1, mul vl]
; CHECK-2048-NEXT:    str z0, [x1, #1, mul vl]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i16, ptr %ldptr, i64 128
  %stoff = getelementptr inbounds nuw i16, ptr %stptr, i64 128
  %x = load <vscale x 8 x i16>, ptr %ldoff, align 2
  store <vscale x 8 x i16> %x, ptr %stoff, align 2
  ret void
}

define void @nxv4i32(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv4i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    mov x8, #64 // =0x40
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0, x8, lsl #2]
; CHECK-NEXT:    st1w { z0.s }, p0, [x1, x8, lsl #2]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv4i32:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ldr z0, [x0, #16, mul vl]
; CHECK-128-NEXT:    str z0, [x1, #16, mul vl]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv4i32:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ldr z0, [x0, #8, mul vl]
; CHECK-256-NEXT:    str z0, [x1, #8, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv4i32:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ldr z0, [x0, #4, mul vl]
; CHECK-512-NEXT:    str z0, [x1, #4, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv4i32:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ldr z0, [x0, #2, mul vl]
; CHECK-1024-NEXT:    str z0, [x1, #2, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv4i32:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ldr z0, [x0, #1, mul vl]
; CHECK-2048-NEXT:    str z0, [x1, #1, mul vl]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i32, ptr %ldptr, i64 64
  %stoff = getelementptr inbounds nuw i32, ptr %stptr, i64 64
  %x = load <vscale x 4 x i32>, ptr %ldoff, align 4
  store <vscale x 4 x i32> %x, ptr %stoff, align 4
  ret void
}

define void @nxv2i64(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv2i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    mov x8, #32 // =0x20
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0, x8, lsl #3]
; CHECK-NEXT:    st1d { z0.d }, p0, [x1, x8, lsl #3]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv2i64:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ldr z0, [x0, #16, mul vl]
; CHECK-128-NEXT:    str z0, [x1, #16, mul vl]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv2i64:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ldr z0, [x0, #8, mul vl]
; CHECK-256-NEXT:    str z0, [x1, #8, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv2i64:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ldr z0, [x0, #4, mul vl]
; CHECK-512-NEXT:    str z0, [x1, #4, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv2i64:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ldr z0, [x0, #2, mul vl]
; CHECK-1024-NEXT:    str z0, [x1, #2, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv2i64:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ldr z0, [x0, #1, mul vl]
; CHECK-2048-NEXT:    str z0, [x1, #1, mul vl]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i64, ptr %ldptr, i64 32
  %stoff = getelementptr inbounds nuw i64, ptr %stptr, i64 32
  %x = load <vscale x 2 x i64>, ptr %ldoff, align 8
  store <vscale x 2 x i64> %x, ptr %stoff, align 8
  ret void
}

define void @nxv4i8(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv4i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    mov w8, #32 // =0x20
; CHECK-NEXT:    ld1b { z0.s }, p0/z, [x0, x8]
; CHECK-NEXT:    st1b { z0.s }, p0, [x1, x8]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv4i8:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ptrue p0.s
; CHECK-128-NEXT:    mov w8, #32 // =0x20
; CHECK-128-NEXT:    ld1b { z0.s }, p0/z, [x0, x8]
; CHECK-128-NEXT:    st1b { z0.s }, p0, [x1, x8]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv4i8:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ptrue p0.s
; CHECK-256-NEXT:    ld1b { z0.s }, p0/z, [x0, #4, mul vl]
; CHECK-256-NEXT:    st1b { z0.s }, p0, [x1, #4, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv4i8:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ptrue p0.s
; CHECK-512-NEXT:    ld1b { z0.s }, p0/z, [x0, #2, mul vl]
; CHECK-512-NEXT:    st1b { z0.s }, p0, [x1, #2, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv4i8:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ptrue p0.s
; CHECK-1024-NEXT:    ld1b { z0.s }, p0/z, [x0, #1, mul vl]
; CHECK-1024-NEXT:    st1b { z0.s }, p0, [x1, #1, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv4i8:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ptrue p0.s
; CHECK-2048-NEXT:    mov w8, #32 // =0x20
; CHECK-2048-NEXT:    ld1b { z0.s }, p0/z, [x0, x8]
; CHECK-2048-NEXT:    st1b { z0.s }, p0, [x1, x8]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i8, ptr %ldptr, i64 32
  %stoff = getelementptr inbounds nuw i8, ptr %stptr, i64 32
  %x = load <vscale x 4 x i8>, ptr %ldoff, align 1
  store <vscale x 4 x i8> %x, ptr %stoff, align 1
  ret void
}

define void @nxv2f32(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv2f32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    mov x8, #16 // =0x10
; CHECK-NEXT:    ld1w { z0.d }, p0/z, [x0, x8, lsl #2]
; CHECK-NEXT:    st1w { z0.d }, p0, [x1, x8, lsl #2]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv2f32:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ptrue p0.d
; CHECK-128-NEXT:    mov x8, #16 // =0x10
; CHECK-128-NEXT:    ld1w { z0.d }, p0/z, [x0, x8, lsl #2]
; CHECK-128-NEXT:    st1w { z0.d }, p0, [x1, x8, lsl #2]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv2f32:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ptrue p0.d
; CHECK-256-NEXT:    ld1w { z0.d }, p0/z, [x0, #4, mul vl]
; CHECK-256-NEXT:    st1w { z0.d }, p0, [x1, #4, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv2f32:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ptrue p0.d
; CHECK-512-NEXT:    ld1w { z0.d }, p0/z, [x0, #2, mul vl]
; CHECK-512-NEXT:    st1w { z0.d }, p0, [x1, #2, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv2f32:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ptrue p0.d
; CHECK-1024-NEXT:    ld1w { z0.d }, p0/z, [x0, #1, mul vl]
; CHECK-1024-NEXT:    st1w { z0.d }, p0, [x1, #1, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv2f32:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ptrue p0.d
; CHECK-2048-NEXT:    mov x8, #16 // =0x10
; CHECK-2048-NEXT:    ld1w { z0.d }, p0/z, [x0, x8, lsl #2]
; CHECK-2048-NEXT:    st1w { z0.d }, p0, [x1, x8, lsl #2]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i8, ptr %ldptr, i64 64
  %stoff = getelementptr inbounds nuw i8, ptr %stptr, i64 64
  %x = load <vscale x 2 x float>, ptr %ldoff, align 4
  store <vscale x 2 x float> %x, ptr %stoff, align 4
  ret void
}

define void @nxv4f64(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: nxv4f64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    mov x8, #16 // =0x10
; CHECK-NEXT:    add x9, x0, #128
; CHECK-NEXT:    ldr z1, [x9, #1, mul vl]
; CHECK-NEXT:    add x9, x1, #128
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0, x8, lsl #3]
; CHECK-NEXT:    st1d { z0.d }, p0, [x1, x8, lsl #3]
; CHECK-NEXT:    str z1, [x9, #1, mul vl]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: nxv4f64:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    add x8, x0, #128
; CHECK-128-NEXT:    ldr z1, [x0, #8, mul vl]
; CHECK-128-NEXT:    ldr z0, [x8, #1, mul vl]
; CHECK-128-NEXT:    add x8, x1, #128
; CHECK-128-NEXT:    str z0, [x8, #1, mul vl]
; CHECK-128-NEXT:    str z1, [x1, #8, mul vl]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: nxv4f64:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    add x8, x0, #128
; CHECK-256-NEXT:    ldr z1, [x0, #4, mul vl]
; CHECK-256-NEXT:    ldr z0, [x8, #1, mul vl]
; CHECK-256-NEXT:    add x8, x1, #128
; CHECK-256-NEXT:    str z0, [x8, #1, mul vl]
; CHECK-256-NEXT:    str z1, [x1, #4, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: nxv4f64:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    add x8, x0, #128
; CHECK-512-NEXT:    ldr z1, [x0, #2, mul vl]
; CHECK-512-NEXT:    ldr z0, [x8, #1, mul vl]
; CHECK-512-NEXT:    add x8, x1, #128
; CHECK-512-NEXT:    str z0, [x8, #1, mul vl]
; CHECK-512-NEXT:    str z1, [x1, #2, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: nxv4f64:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    add x8, x0, #128
; CHECK-1024-NEXT:    ldr z1, [x0, #1, mul vl]
; CHECK-1024-NEXT:    ldr z0, [x8, #1, mul vl]
; CHECK-1024-NEXT:    add x8, x1, #128
; CHECK-1024-NEXT:    str z0, [x8, #1, mul vl]
; CHECK-1024-NEXT:    str z1, [x1, #1, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: nxv4f64:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ptrue p0.d
; CHECK-2048-NEXT:    mov x8, #16 // =0x10
; CHECK-2048-NEXT:    add x9, x0, #128
; CHECK-2048-NEXT:    ldr z1, [x9, #1, mul vl]
; CHECK-2048-NEXT:    add x9, x1, #128
; CHECK-2048-NEXT:    ld1d { z0.d }, p0/z, [x0, x8, lsl #3]
; CHECK-2048-NEXT:    st1d { z0.d }, p0, [x1, x8, lsl #3]
; CHECK-2048-NEXT:    str z1, [x9, #1, mul vl]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i8, ptr %ldptr, i64 128
  %stoff = getelementptr inbounds nuw i8, ptr %stptr, i64 128
  %x = load <vscale x 4 x double>, ptr %ldoff, align 8
  store <vscale x 4 x double> %x, ptr %stoff, align 8
  ret void
}

define void @v8i32(ptr %ldptr, ptr %stptr) {
; CHECK-LABEL: v8i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldp q0, q1, [x0, #64]
; CHECK-NEXT:    ldp q3, q2, [x0, #32]
; CHECK-NEXT:    stp q0, q1, [x1, #64]
; CHECK-NEXT:    stp q3, q2, [x1, #32]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: v8i32:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    ldp q0, q1, [x0, #64]
; CHECK-128-NEXT:    ldp q3, q2, [x0, #32]
; CHECK-128-NEXT:    stp q0, q1, [x1, #64]
; CHECK-128-NEXT:    stp q3, q2, [x1, #32]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: v8i32:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    ldr z0, [x0, #2, mul vl]
; CHECK-256-NEXT:    ldr z1, [x0, #1, mul vl]
; CHECK-256-NEXT:    str z0, [x1, #2, mul vl]
; CHECK-256-NEXT:    str z1, [x1, #1, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: v8i32:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    ptrue p0.s
; CHECK-512-NEXT:    mov x8, #8 // =0x8
; CHECK-512-NEXT:    ld1w { z0.s }, p0/z, [x0, x8, lsl #2]
; CHECK-512-NEXT:    st1w { z0.s }, p0, [x1, x8, lsl #2]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: v8i32:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    ptrue p0.s, vl16
; CHECK-1024-NEXT:    mov x8, #8 // =0x8
; CHECK-1024-NEXT:    ld1w { z0.s }, p0/z, [x0, x8, lsl #2]
; CHECK-1024-NEXT:    st1w { z0.s }, p0, [x1, x8, lsl #2]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: v8i32:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    ptrue p0.s, vl16
; CHECK-2048-NEXT:    mov x8, #8 // =0x8
; CHECK-2048-NEXT:    ld1w { z0.s }, p0/z, [x0, x8, lsl #2]
; CHECK-2048-NEXT:    st1w { z0.s }, p0, [x1, x8, lsl #2]
; CHECK-2048-NEXT:    ret
  %ldoff = getelementptr inbounds nuw i8, ptr %ldptr, i64 32
  %stoff = getelementptr inbounds nuw i8, ptr %stptr, i64 32
  %x = load <16 x i32>, ptr %ldoff, align 4
  store <16 x i32> %x, ptr %stoff, align 4
  ret void
}

define void @v8i32_vscale(ptr %0) {
; CHECK-LABEL: v8i32_vscale:
; CHECK:       // %bb.0:
; CHECK-NEXT:    movi v0.4s, #1
; CHECK-NEXT:    rdvl x8, #2
; CHECK-NEXT:    add x8, x0, x8
; CHECK-NEXT:    stp q0, q0, [x8]
; CHECK-NEXT:    ret
;
; CHECK-128-LABEL: v8i32_vscale:
; CHECK-128:       // %bb.0:
; CHECK-128-NEXT:    movi v0.4s, #1
; CHECK-128-NEXT:    rdvl x8, #2
; CHECK-128-NEXT:    add x8, x0, x8
; CHECK-128-NEXT:    stp q0, q0, [x8]
; CHECK-128-NEXT:    ret
;
; CHECK-256-LABEL: v8i32_vscale:
; CHECK-256:       // %bb.0:
; CHECK-256-NEXT:    mov z0.s, #1 // =0x1
; CHECK-256-NEXT:    str z0, [x0, #2, mul vl]
; CHECK-256-NEXT:    ret
;
; CHECK-512-LABEL: v8i32_vscale:
; CHECK-512:       // %bb.0:
; CHECK-512-NEXT:    mov z0.s, #1 // =0x1
; CHECK-512-NEXT:    ptrue p0.s, vl8
; CHECK-512-NEXT:    st1w { z0.s }, p0, [x0, #2, mul vl]
; CHECK-512-NEXT:    ret
;
; CHECK-1024-LABEL: v8i32_vscale:
; CHECK-1024:       // %bb.0:
; CHECK-1024-NEXT:    mov z0.s, #1 // =0x1
; CHECK-1024-NEXT:    ptrue p0.s, vl8
; CHECK-1024-NEXT:    st1w { z0.s }, p0, [x0, #2, mul vl]
; CHECK-1024-NEXT:    ret
;
; CHECK-2048-LABEL: v8i32_vscale:
; CHECK-2048:       // %bb.0:
; CHECK-2048-NEXT:    mov z0.s, #1 // =0x1
; CHECK-2048-NEXT:    ptrue p0.s, vl8
; CHECK-2048-NEXT:    st1w { z0.s }, p0, [x0, #2, mul vl]
; CHECK-2048-NEXT:    ret
  %vl = call i64 @llvm.vscale()
  %vlx = shl i64 %vl, 5
  %2 = getelementptr inbounds nuw i8, ptr %0, i64 %vlx
  store <8 x i32> splat (i32 1), ptr %2, align 4
  ret void
}
