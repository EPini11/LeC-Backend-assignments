; ModuleID = 'TestAI.c'
source_filename = "TestAI.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @Test1() #0 {

  %1 = add i32 10, 10
  
  %2 = add i32 %1, 11
  %3 = sub i32 11, %2
  %4 = alloca i32
  store i32 %3, i32* %4 ; %3 deve diventare il risultato di sub 0 %1

  %5 = add i32 11, %1
  %6 = sub i32 11, %2
  %7 = alloca i32
  store i32 %6, i32* %7 ; %6 deve diventare il risultato di sub 0 %1
  
  %8 = add i32 %1, 11
  %9 = sub i32 %8, 11
  %10 = alloca i32
  store i32 %9, i32* %10 ; %9 deve diventare %1

  %11 = add i32 11, %1
  %12 = sub i32 %11, 11
  %13 = alloca i32
  store i32 %12, i32* %13 ; %12 deve diventare %1
  
  %14 = sub i32 %1, 11
  %15 = add i32 %14, 11
  %16 = alloca i32
  store i32 %15, i32* %16 ; %15 deve diventare %1
  
  %17 = sub i32 %1, 11
  %18 = add i32 11, %14
  %19 = alloca i32
  store i32 %18, i32* %19 ; %18 deve diventare %1

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
