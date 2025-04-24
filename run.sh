qemu-system-aarch64 -machine raspi4b -nographic -kernel ./build/kernel8.img
qemu-system-aarch64 -machine raspi4b -display gtk -kernel ./build/kernel8.img -serial stdio