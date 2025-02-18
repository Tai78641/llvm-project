// This is crashing in CI "most of the time" on a AMD Rome CPU VM on GCP with:
//    Tracer caught signal 11: addr=0x7a800028 pc=0x2e81ba sp=0x7efd2a7ffd50
//    LeakSanitizer has encountered a fatal error.
// This is hard to reproduce locally unfortunately. Disable it with ASAN/LSAN
// to keep the bot green for now.
// RUN: export LSAN_OPTIONS=detect_leaks=0

// RUN:   mlir-opt %s -pass-pipeline="builtin.module(async-to-async-runtime,func.func(async-runtime-ref-counting,async-runtime-ref-counting-opt),convert-async-to-llvm,func.func(convert-arith-to-llvm),convert-func-to-llvm,convert-cf-to-llvm,reconcile-unrealized-casts)" \
// RUN: | mlir-runner                                                      \
// RUN:     -e main -entry-point-result=void -O0                               \
// RUN:     -shared-libs=%mlir_c_runner_utils  \
// RUN:     -shared-libs=%mlir_runner_utils    \
// RUN:     -shared-libs=%mlir_async_runtime   \
// RUN: | FileCheck %s

// FIXME: https://github.com/llvm/llvm-project/issues/57231
// UNSUPPORTED: hwasan
// FIXME: Windows does not have aligned_alloc
// UNSUPPORTED: system-windows

func.func @main() {
  %c1 = arith.constant 1 : index
  %c5 = arith.constant 5 : index

  %group = async.create_group %c5 : !async.group

  %token0 = async.execute { async.yield }
  %token1 = async.execute { async.yield }
  %token2 = async.execute { async.yield }
  %token3 = async.execute { async.yield }
  %token4 = async.execute { async.yield }

  %0 = async.add_to_group %token0, %group : !async.token
  %1 = async.add_to_group %token1, %group : !async.token
  %2 = async.add_to_group %token2, %group : !async.token
  %3 = async.add_to_group %token3, %group : !async.token
  %4 = async.add_to_group %token4, %group : !async.token

  %token5 = async.execute {
    async.await_all %group
    async.yield
  }

  %group0 = async.create_group %c1 : !async.group
  %5 = async.add_to_group %token5, %group0 : !async.token
  async.await_all %group0

  // CHECK: Current thread id: [[THREAD:.*]]
  call @mlirAsyncRuntimePrintCurrentThreadId(): () -> ()

  return
}

func.func private @mlirAsyncRuntimePrintCurrentThreadId() -> ()
