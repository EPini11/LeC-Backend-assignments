#include <stdio.h>

void foo(int c, int z) {
  int a = 9, h, m = 0, n = 0, q, r = 0, y = 0;

LOOP:
  z = z + 1; // %4 = add nsw i32 %.0, 1
  y = c + 3; // %5 = add nsw i32 %0, 3
  q = c + 7; // %6 = add nsw i32 %0, 7
  if (z < 5) {
    a = a + 2; // %9 = add nsw i32 %.01, 2
    h = c + 3; // %10 = add nsw i32 %0, 3
  } else {
    a = a - 1; // %12 = sub nsw i32 %.01, 1
    h = c + 4; // %13 = add nsw i32 %0, 4
    if (z >= 10) {
      goto EXIT;
    }
  }
  m = y + 7; // %18 = add nsw i32 %5, 7
  n = h + 2; // %19 = add nsw i32 %.02, 2
  y = c + 7; // %20 = add nsw i32 %0, 7
  r = q + 5; // %21 = add nsw i32 %6, 5
  goto LOOP; 
EXIT:
  printf("%d,%d,%d,%d\n", a, h, q, z);
}

int main() {
  foo(0, 4);
  foo(0, 12);
  return 0;
}