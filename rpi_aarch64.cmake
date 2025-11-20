set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(ARMGNU aarch64-linux-gnu)
set(TOOLCHAIN_PREFIX ${ARMGNU}-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)

set(CMAKE_C_FLAGS "-ffreestanding -nostdlib -nostartfiles -fno-stack-protector -mgeneral-regs-only -Wall -Wextra")
set(CMAKE_ASM_FLAGS "")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2")
