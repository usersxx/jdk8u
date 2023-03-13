/*
*
*/

#ifndef OS_CPU_BSD_AARCH64_VM_BYTES_BSD_AARCH64_INLINE_HPP
#define OS_CPU_BSD_AARCH64_VM_BYTES_BSD_AARCH64_INLINE_HPP

#ifdef __APPLE__
#include <libkern/OSByteOrder.h>
#endif

#if defined(__APPLE__)
#  define bswap_16(x) OSSwapInt16(x)
#  define bswap_32(x) OSSwapInt32(x)
#  define bswap_64(x) OSSwapInt64(x)
#else
#  error "Unimplemented"
#endif

// Efficient swapping of data bytes from Java byte
// ordering to native byte ordering and vice versa.
inline u2   Bytes::swap_u2(u2 x) {
  return bswap_16(x);
}

inline u4   Bytes::swap_u4(u4 x) {
  return bswap_32(x);
}

inline u8 Bytes::swap_u8(u8 x) {
  return bswap_64(x);
}

#endif // OS_CPU_BSD_AARCH64_VM_BYTES_BSD_AARCH64_INLINE_HPP
