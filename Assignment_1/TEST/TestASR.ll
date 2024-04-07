; ModuleID = 'TestAI.c'
source_filename = "TestAI.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @Test1() #0 {

  %1 = add i32 10, 10
  
  %2 = mul i32 %1, 16
  %3 = mul i32 16, %1
  %4 = mul i32 15, 16
  %5 = mul i32 16, 15
  %6 = mul i32 17, 16
  %7 = mul i32 16, 17
  
  %8 = mul i32 %1, 15
  %9 = mul i32 15, %1
  %10 = mul i32 11, 15
  %11 = mul i32 15, 11
  
  %12 = mul i32 %1, 17
  %13 = mul i32 17, %1
  %14 = mul i32 11, 17
  %15 = mul i32 17, 11
  
  %16 = sdiv i32 %1, 16
  %17 = sdiv i32 20, 16
  
  ret void
  
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.6"}
