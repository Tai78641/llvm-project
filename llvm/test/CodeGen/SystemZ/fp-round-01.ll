; Test rounding functions for z10.
;
; RUN: llc < %s -mtriple=s390x-linux-gnu -mcpu=z10 | FileCheck %s

; Test rint for f32.
declare float @llvm.rint.f32(float %f)
define float @f1(float %f) {
; CHECK-LABEL: f1:
; CHECK: fiebr %f0, 0, %f0
; CHECK: br %r14
  %res = call float @llvm.rint.f32(float %f)
  ret float %res
}

; Test rint for f64.
declare double @llvm.rint.f64(double %f)
define double @f2(double %f) {
; CHECK-LABEL: f2:
; CHECK: fidbr %f0, 0, %f0
; CHECK: br %r14
  %res = call double @llvm.rint.f64(double %f)
  ret double %res
}

; Test rint for f128.
declare fp128 @llvm.rint.f128(fp128 %f)
define void @f3(ptr %ptr) {
; CHECK-LABEL: f3:
; CHECK: fixbr %f0, 0, %f0
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.rint.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}

; Test nearbyint for f16.
declare half @llvm.nearbyint.f16(half %f)
define half @f4_half(half %f) {
; CHECK-LABEL: f4_half:
; CHECK: brasl %r14, __extendhfsf2@PLT
; CHECK: brasl %r14, nearbyintf@PLT
; CHECK: brasl %r14, __truncsfhf2@PLT
; CHECK: br %r14
  %res = call half @llvm.nearbyint.f16(half %f)
  ret half %res
}

; Test nearbyint for f32.
declare float @llvm.nearbyint.f32(float %f)
define float @f4(float %f) {
; CHECK-LABEL: f4:
; CHECK: brasl %r14, nearbyintf@PLT
; CHECK: br %r14
  %res = call float @llvm.nearbyint.f32(float %f)
  ret float %res
}

; Test nearbyint for f64.
declare double @llvm.nearbyint.f64(double %f)
define double @f5(double %f) {
; CHECK-LABEL: f5:
; CHECK: brasl %r14, nearbyint@PLT
; CHECK: br %r14
  %res = call double @llvm.nearbyint.f64(double %f)
  ret double %res
}

; Test nearbyint for f128.
declare fp128 @llvm.nearbyint.f128(fp128 %f)
define void @f6(ptr %ptr) {
; CHECK-LABEL: f6:
; CHECK: brasl %r14, nearbyintl@PLT
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.nearbyint.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}

; Test floor for f16.
declare half @llvm.floor.f16(half %f)
define half @f7_half(half %f) {
; CHECK-LABEL: f7_half:
; CHECK: brasl %r14, __extendhfsf2@PLT
; CHECK: brasl %r14, floorf@PLT
; CHECK: brasl %r14, __truncsfhf2@PLT
; CHECK: br %r14
  %res = call half @llvm.floor.f16(half %f)
  ret half %res
}

; Test floor for f32.
declare float @llvm.floor.f32(float %f)
define float @f7(float %f) {
; CHECK-LABEL: f7:
; CHECK: brasl %r14, floorf@PLT
; CHECK: br %r14
  %res = call float @llvm.floor.f32(float %f)
  ret float %res
}

; Test floor for f64.
declare double @llvm.floor.f64(double %f)
define double @f8(double %f) {
; CHECK-LABEL: f8:
; CHECK: brasl %r14, floor@PLT
; CHECK: br %r14
  %res = call double @llvm.floor.f64(double %f)
  ret double %res
}

; Test floor for f128.
declare fp128 @llvm.floor.f128(fp128 %f)
define void @f9(ptr %ptr) {
; CHECK-LABEL: f9:
; CHECK: brasl %r14, floorl@PLT
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.floor.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}

; Test ceil for f16.
declare half @llvm.ceil.f16(half %f)
define half @f10_half(half %f) {
; CHECK-LABEL: f10_half:
; CHECK: brasl %r14, __extendhfsf2@PLT
; CHECK: brasl %r14, ceilf@PLT
; CHECK: brasl %r14, __truncsfhf2@PLT
; CHECK: br %r14
  %res = call half @llvm.ceil.f16(half %f)
  ret half %res
}

; Test ceil for f32.
declare float @llvm.ceil.f32(float %f)
define float @f10(float %f) {
; CHECK-LABEL: f10:
; CHECK: brasl %r14, ceilf@PLT
; CHECK: br %r14
  %res = call float @llvm.ceil.f32(float %f)
  ret float %res
}

; Test ceil for f64.
declare double @llvm.ceil.f64(double %f)
define double @f11(double %f) {
; CHECK-LABEL: f11:
; CHECK: brasl %r14, ceil@PLT
; CHECK: br %r14
  %res = call double @llvm.ceil.f64(double %f)
  ret double %res
}

; Test ceil for f128.
declare fp128 @llvm.ceil.f128(fp128 %f)
define void @f12(ptr %ptr) {
; CHECK-LABEL: f12:
; CHECK: brasl %r14, ceill@PLT
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.ceil.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}

; Test trunc for f32.
declare float @llvm.trunc.f32(float %f)
define float @f13(float %f) {
; CHECK-LABEL: f13:
; CHECK: brasl %r14, truncf@PLT
; CHECK: br %r14
  %res = call float @llvm.trunc.f32(float %f)
  ret float %res
}

; Test trunc for f64.
declare double @llvm.trunc.f64(double %f)
define double @f14(double %f) {
; CHECK-LABEL: f14:
; CHECK: brasl %r14, trunc@PLT
; CHECK: br %r14
  %res = call double @llvm.trunc.f64(double %f)
  ret double %res
}

; Test trunc for f128.
declare fp128 @llvm.trunc.f128(fp128 %f)
define void @f15(ptr %ptr) {
; CHECK-LABEL: f15:
; CHECK: brasl %r14, truncl@PLT
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.trunc.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}

; Test round for f16.
declare half @llvm.round.f16(half %f)
define half @f16_half(half %f) {
; CHECK-LABEL: f16_half:
; CHECK: brasl %r14, __extendhfsf2@PLT
; CHECK: brasl %r14, roundf@PLT
; CHECK: brasl %r14, __truncsfhf2@PLT
; CHECK: br %r14
  %res = call half @llvm.round.f16(half %f)
  ret half %res
}

; Test round for f32.
declare float @llvm.round.f32(float %f)
define float @f16(float %f) {
; CHECK-LABEL: f16:
; CHECK: brasl %r14, roundf@PLT
; CHECK: br %r14
  %res = call float @llvm.round.f32(float %f)
  ret float %res
}

; Test round for f64.
declare double @llvm.round.f64(double %f)
define double @f17(double %f) {
; CHECK-LABEL: f17:
; CHECK: brasl %r14, round@PLT
; CHECK: br %r14
  %res = call double @llvm.round.f64(double %f)
  ret double %res
}

; Test round for f128.
declare fp128 @llvm.round.f128(fp128 %f)
define void @f18(ptr %ptr) {
; CHECK-LABEL: f18:
; CHECK: brasl %r14, roundl@PLT
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.round.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}

; Test roundeven for f32.
declare float @llvm.roundeven.f32(float %f)
define float @f19(float %f) {
; CHECK-LABEL: f19:
; CHECK: brasl %r14, roundevenf@PLT
; CHECK: br %r14
  %res = call float @llvm.roundeven.f32(float %f)
  ret float %res
}

; Test roundeven for f64.
declare double @llvm.roundeven.f64(double %f)
define double @f20(double %f) {
; CHECK-LABEL: f20:
; CHECK: brasl %r14, roundeven@PLT
; CHECK: br %r14
  %res = call double @llvm.roundeven.f64(double %f)
  ret double %res
}

; Test roundeven for f128.
declare fp128 @llvm.roundeven.f128(fp128 %f)
define void @f21(ptr %ptr) {
; CHECK-LABEL: f21:
; CHECK: brasl %r14, roundevenl@PLT
; CHECK: br %r14
  %src = load fp128, ptr %ptr
  %res = call fp128 @llvm.roundeven.f128(fp128 %src)
  store fp128 %res, ptr %ptr
  ret void
}
