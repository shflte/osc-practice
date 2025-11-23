cmake_minimum_required(VERSION 3.10)
project(RPI_OS_BOOT C ASM)

# UART bootloader
set(BL_TARGET bootloader.img)

add_executable(bootloader EXCLUDE_FROM_ALL
    bootloader/hw_utils.S
    bootloader/mm.S
    bootloader/uart.c
    bootloader/utils.c
    bootloader/init.S
    bootloader/main.c
)

set_target_properties(bootloader PROPERTIES SUFFIX ".elf")

target_include_directories(bootloader PUBLIC include)

target_link_options(bootloader PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/bootloader/linker.ld"
)

add_custom_command(
    TARGET bootloader POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS $<TARGET_FILE:bootloader> -O binary ${CMAKE_CURRENT_BINARY_DIR}/${BL_TARGET}
    COMMENT "Generating ${BL_TARGET}"
)

# Kernel
set(KERNEL_TARGET kernel8.img)

add_executable(kernel8
    src/utils.S
    src/mm.S
    src/uart.c
    src/utils.c
    src/init.S
    src/main.c
    src/shell.c
    src/mailbox.c
    src/reboot.c
    src/cpio.c
    src/mm.c
    src/fdt.c
    src/entry.S
    src/exception.c
    src/timer.c
    src/irq.c
)

set_target_properties(kernel8 PROPERTIES SUFFIX ".elf")

target_include_directories(kernel8 PUBLIC include)

target_link_options(kernel8 PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld"
)

add_custom_command(
    TARGET kernel8 POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS $<TARGET_FILE:kernel8> -O binary ${CMAKE_CURRENT_BINARY_DIR}/${KERNEL_TARGET}
    COMMENT "Generating ${KERNEL_TARGET}"
)

add_custom_target(run
    COMMAND qemu-system-aarch64
        -M raspi3b
        -kernel kernel8.img
        -initrd initramfs.cpio
        -dtb bcm2710-rpi-3-b-plus.dtb
        -display none
        -serial null
        -serial stdio

    DEPENDS kernel8
    COMMENT "Running QEMU for kernel..."
)

add_custom_target(debug
    COMMAND qemu-system-aarch64
        -M raspi3b
        -kernel kernel8.img
        -initrd initramfs.cpio
        -dtb bcm2710-rpi-3-b-plus.dtb
        -display none
        -serial null
        -serial tcp::4321,server,nowait
        -s -S

    DEPENDS kernel8
    COMMENT "Running QEMU for kernel..."
)
