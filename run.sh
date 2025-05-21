qemu-system-aarch64 -machine raspi4b -nographic -kernel ./build/kernel8.img # run with terminal
qemu-system-aarch64 -machine raspi4b -display gtk -kernel ./build/kernel8.img -serial stdio # run with graphic & terminal
qemu-system-aarch64 -s -S -machine raspi4b -nographic -kernel ./build/kernel8.img # run with gdb