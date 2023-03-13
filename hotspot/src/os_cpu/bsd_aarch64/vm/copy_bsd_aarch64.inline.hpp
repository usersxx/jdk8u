ifndef OS_CPU_BSD_AARCH64_VM_COPY_BSD_AARCH64_INLINE_HPP
#define OS_CPU_BSD_AARCH64_VM_COPY_BSD_AARCH64_INLINE_HPP

#define COPY_SMALL(from, to, count)                                     \
{                                                                       \
        long tmp0, tmp1, tmp2, tmp3;                                    \
        long tmp4, tmp5, tmp6, tmp7;                                    \
  __asm volatile(                                                       \
"       adr     %[t0], 0f;\n"                                           \
"       add     %[t0], %[t0], %[cnt], lsl #5;\n"                        \
"       br      %[t0];\n"                                               \
"       .align  5;\n"                                                   \
"0:"                                                                    \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldr     %[t0], [%[s], #0];\n"                                   \
"       str     %[t0], [%[d], #0];\n"                                   \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldp     %[t0], %[t1], [%[s], #0];\n"                            \
"       stp     %[t0], %[t1], [%[d], #0];\n"                            \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldp     %[t0], %[t1], [%[s], #0];\n"                            \
"       ldr     %[t2], [%[s], #16];\n"                                  \
"       stp     %[t0], %[t1], [%[d], #0];\n"                            \
"       str     %[t2], [%[d], #16];\n"                                  \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldp     %[t0], %[t1], [%[s], #0];\n"                            \
"       ldp     %[t2], %[t3], [%[s], #16];\n"                           \
"       stp     %[t0], %[t1], [%[d], #0];\n"                            \
"       stp     %[t2], %[t3], [%[d], #16];\n"                           \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldp     %[t0], %[t1], [%[s], #0];\n"                            \
"       ldp     %[t2], %[t3], [%[s], #16];\n"                           \
"       ldr     %[t4], [%[s], #32];\n"                                  \
"       stp     %[t0], %[t1], [%[d], #0];\n"                            \
"       stp     %[t2], %[t3], [%[d], #16];\n"                           \
"       str     %[t4], [%[d], #32];\n"                                  \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldp     %[t0], %[t1], [%[s], #0];\n"                            \
"       ldp     %[t2], %[t3], [%[s], #16];\n"                           \
"       ldp     %[t4], %[t5], [%[s], #32];\n"                           \
"2:"                                                                    \
"       stp     %[t0], %[t1], [%[d], #0];\n"                            \
"       stp     %[t2], %[t3], [%[d], #16];\n"                           \
"       stp     %[t4], %[t5], [%[d], #32];\n"                           \
"       b       1f;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldr     %[t6], [%[s], #0];\n"                                   \
"       ldp     %[t0], %[t1], [%[s], #8];\n"                            \
"       ldp     %[t2], %[t3], [%[s], #24];\n"                           \
"       ldp     %[t4], %[t5], [%[s], #40];\n"                           \
"       str     %[t6], [%[d]], #8;\n"                                   \
"       b       2b;\n"                                                  \
"       .align  5;\n"                                                   \
"       ldp     %[t0], %[t1], [%[s], #0];\n"                            \
"       ldp     %[t2], %[t3], [%[s], #16];\n"                           \
"       ldp     %[t4], %[t5], [%[s], #32];\n"                           \
"       ldp     %[t6], %[t7], [%[s], #48];\n"                           \
"       stp     %[t0], %[t1], [%[d], #0];\n"                            \
"       stp     %[t2], %[t3], [%[d], #16];\n"                           \
"       stp     %[t4], %[t5], [%[d], #32];\n"                           \
"       stp     %[t6], %[t7], [%[d], #48];\n"                           \
"1:"                                                                    \
                                                                        \
  : [s]"+r"(from), [d]"+r"(to), [cnt]"+r"(count),                       \
    [t0]"=&r"(tmp0), [t1]"=&r"(tmp1), [t2]"=&r"(tmp2), [t3]"=&r"(tmp3), \
    [t4]"=&r"(tmp4), [t5]"=&r"(tmp5), [t6]"=&r"(tmp6), [t7]"=&r"(tmp7)  \
  :                                                                     \
  : "memory", "cc");                                                    \
}

static void pd_conjoint_words(HeapWord* from, HeapWord* to, size_t count) {
  __asm volatile( "prfm pldl1strm, [%[s], #0];" :: [s]"r"(from) : "memory");
  if (__builtin_expect(count <= 8, 1)) {
    COPY_SMALL(from, to, count);
    return;
  }
  _Copy_conjoint_words(from, to, count);
}

static void pd_disjoint_words(HeapWord* from, HeapWord* to, size_t count) {
  if (__builtin_constant_p(count)) {
    memcpy(to, from, count * sizeof(HeapWord));
    return;
  }
  __asm volatile( "prfm pldl1strm, [%[s], #0];" :: [s]"r"(from) : "memory");
  if (__builtin_expect(count <= 8, 1)) {
    COPY_SMALL(from, to, count);
    return;
  }
  _Copy_disjoint_words(from, to, count);
}

static void pd_disjoint_words_atomic(HeapWord* from, HeapWord* to, size_t count) {
  __asm volatile( "prfm pldl1strm, [%[s], #0];" :: [s]"r"(from) : "memory");
  if (__builtin_expect(count <= 8, 1)) {
    COPY_SMALL(from, to, count);
    return;
  }
  _Copy_disjoint_words(from, to, count);
}

static void pd_aligned_conjoint_words(HeapWord* from, HeapWord* to, size_t count) {
  pd_conjoint_words(from, to, count);
}

static void pd_aligned_disjoint_words(HeapWord* from, HeapWord* to, size_t count) {
  pd_disjoint_words(from, to, count);
}

static void pd_conjoint_bytes(void* from, void* to, size_t count) {
  (void)memmove(to, from, count);
}

static void pd_conjoint_bytes_atomic(void* from, void* to, size_t count) {
  pd_conjoint_bytes(from, to, count);
}

static void pd_conjoint_jshorts_atomic(jshort* from, jshort* to, size_t count) {
  _Copy_conjoint_jshorts_atomic(from, to, count);
}

static void pd_conjoint_jints_atomic(jint* from, jint* to, size_t count) {
  _Copy_conjoint_jints_atomic(from, to, count);
}

static void pd_conjoint_jlongs_atomic(jlong* from, jlong* to, size_t count) {
  _Copy_conjoint_jlongs_atomic(from, to, count);
}

static void pd_conjoint_oops_atomic(oop* from, oop* to, size_t count) {
  assert(BytesPerLong == BytesPerOop, "jlongs and oops must be the same size");
  _Copy_conjoint_jlongs_atomic((jlong*)from, (jlong*)to, count);
}

static void pd_arrayof_conjoint_bytes(HeapWord* from, HeapWord* to, size_t count) {
  _Copy_arrayof_conjoint_bytes(from, to, count);
}

static void pd_arrayof_conjoint_jshorts(HeapWord* from, HeapWord* to, size_t count) {
  _Copy_arrayof_conjoint_jshorts(from, to, count);
}

static void pd_arrayof_conjoint_jints(HeapWord* from, HeapWord* to, size_t count) {
   _Copy_arrayof_conjoint_jints(from, to, count);
}

static void pd_arrayof_conjoint_jlongs(HeapWord* from, HeapWord* to, size_t count) {
  _Copy_arrayof_conjoint_jlongs(from, to, count);
}

static void pd_arrayof_conjoint_oops(HeapWord* from, HeapWord* to, size_t count) {
  assert(!UseCompressedOops, "foo!");
  assert(BytesPerLong == BytesPerOop, "jlongs and oops must be the same size");
  _Copy_arrayof_conjoint_jlongs(from, to, count);
}

#endif // OS_CPU_BSD_AARCH64_VM_COPY_BSD_AARCH64_INLINE_HPP
