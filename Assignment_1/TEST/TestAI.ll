; ModuleID = 'TestAI.c'
source_filename = "TestAI.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @Test1() #0 {

  %1 = add i32 10, 10
  %2 = add i32 %1, 0
  %3 = add i32 10, 10
  %4 = add i32 0, %3
  %5 = add i32 %2, %4 ; to %5 = add i32 %1, %3

  %6 = add i32 10, 0
  %7 = add i32 0, 10
  %8 = add i32 %6, %7 ; to %8 = add i32 10, 10
  
  %9 = add i32 20, 20
  %10 = mul i32 %9, 1
  %11 = add i32 20, 20
  %12 = mul i32 1, %11
  %13 = add i32 %10, %12 ; to %13 = add i32 %9, %11

  %14 = mul i32 20, 1
  %15 = mul i32 1, 20
  %16 = add i32 %14, %15 ; to %16 = add i32 20, 20
  
  %17 = add i32 30, 30
  %18 = sub i32 %17, 0
  %19 = sub i32 30, 0
  %20 = add i32 %18, %19 ; to %20 = add i32 %17, 30

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
