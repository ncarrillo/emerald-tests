#include <stdio.h>
#include <stdint.h>

void set_sr(uint32_t value) {
    __asm__ __volatile__(
        "mov.l %0, r1\n"  // set r1 to the value we want in sr
        "ldc r1, sr\n"    // load r1 into sr
        :
        : "m" (value)
        : "r1"
    );
}

uint32_t div1_instruction(uint32_t value, uint32_t sr_value) {
    // set sr to the starting value
    set_sr(sr_value);

    __asm__ __volatile__(
        "mov.l %1, r0\n"     // use r0 as the operand to div1
        "stc sr, r1\n"       // save sr in r1
        "mov.l r1, @-r15\n"  // push r1
        "mov.l r14, @-r15\n" // push r14
        "stc gbr, r2\n"      // save gbr
        "mov.l r2, @-r15\n"  // push gbr
        "stc vbr, r3\n"      // save vbr
        "mov.l r3, @-r15\n"  // push vbr

        "div1 r0, r0\n"      // div1

        "mov.l @r15+, r3\n"  // pop vbr
        "ldc r3, vbr\n"      // restore vbr
        "mov.l @r15+, r2\n"  // pop gbr
        "ldc r2, gbr\n"      // restore gbr
        "mov.l @r15+, r14\n" // pop r14
        "mov.l @r15+, r1\n"  // pop sr
        "ldc r1, sr\n"       // restore sr

        "mov.l r0, %0\n"
        : "=m" (value)
        : "m" (value)
        : "r0", "r1", "r2", "r3", "r14", "memory"
    );

    return value;
}

int main() {
    uint32_t value = 0xb1e12951;

    printf("initial value: 0x%08x\n", value);

    uint32_t result = div1_instruction(value, 0x10000271);

    printf("value after div1: 0x%08x\n", value);
    return 0;
}
