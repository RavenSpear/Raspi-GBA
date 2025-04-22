cmake_minimum_required(VERSION 3.22)
project(raspi-gba LANGUAGES C ASM) # ASM must set, or boot.S wont be compiled

set(AARCH64_TOOLCHAIN_DIR /opt/toolchains/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin)
set(CMAKE_C_COMPILER ${AARCH64_TOOLCHAIN_DIR}/aarch64-none-elf-gcc)
set(CMAKE_ASM_COMPILER ${AARCH64_TOOLCHAIN_DIR}/aarch64-none-elf-gcc)
set(CMAKE_LINKER ${AARCH64_TOOLCHAIN_DIR}/aarch64-none-elf-ld)
set(CMAKE_OBJCOPY ${AARCH64_TOOLCHAIN_DIR}/aarch64-none-elf-objcopy)

set(TARGET_KERNEL kernel8)

# -ffreestanding, assume a none-standard-C env
# -nostdlib, dont link stdlib
# -nostartfiles, dont use standard startup file, such as crt0.o
# -mstrict-align, strictly align
add_compile_options(-Wall -O2 -ffreestanding -nostdlib -nostartfiles -mstrict-align)

add_executable(${TARGET_KERNEL} boot.S kernel.c pl011.c)
# add link scripts
set_target_properties(
    ${TARGET_KERNEL} PROPERTIES 
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/link.ld -nostdlib"
    SUFFIX ".elf"
)
# objcopy -O binary *.elf *.img
add_custom_command(
    TARGET ${TARGET_KERNEL}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_KERNEL}.elf ${TARGET_KERNEL}.img
)
