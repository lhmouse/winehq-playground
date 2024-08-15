// x86_64-w64-mingw32-gcc test.c -Wall -Wextra -O2 -lntdll
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

int
main(void)
  {
    LARGE_INTEGER t1, t2;

    // Get current time and round up to a one-second boundary.
    GetSystemTimeAsFileTime((FILETIME*) &t1);
    t1.QuadPart /= 10000000;
    t1.QuadPart *= 10000000;

    for(int i = 0;  i < 10;  ++i) {
      t1.QuadPart += 20000000;

      // Wait for the current process itself (effectively sleep).
      printf("- sleep until  %lld\n", t1.QuadPart);
      NtWaitForSingleObject((HANDLE) -1, FALSE, &t1);

      // Check current time.
      GetSystemTimeAsFileTime((FILETIME*) &t2);
      const char* msg = "\0(bug)" + (t1.QuadPart > t2.QuadPart);
      printf("  back; now    %lld %s\n", t2.QuadPart, msg);
    }
  }
