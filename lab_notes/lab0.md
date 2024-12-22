[Spec](https://nycu-caslab.github.io/OSC2024/labs/lab0.html)

## Environment Setup
檢查 cross compiler 跟 cross debugger
```
# cross compiler
aarch64-linux-gnu-gcc --version

# cross debugger
gdb-multiarch --version
# or
aarch64-linux-gnu-gdb --version
```

檢查 QEMU
```
qemu-system-aarch64 --version
```

## Source Code & Linker Script Explained
`a.S`:
```
.section ".text"
_start:
	nop
	b _start
```
`.section ".text"` specify 接下來的 code 屬於 `.text` section。

`linker.ld`:
```
SECTIONS
{
  . = 0x80000;
  .text : { *(.text) }
}
```
- `. = ...` 是在 specify 接下來的 code 要從哪個 logical address 開始 load。
- 第一個 `.text` 只是 section name。
- 第二個 `.text` 指向 `a.S` 的 `.text` section，`*` 會讓所有 source file 的 `.text` section 被按照在 link command 中的順序被 load 進來。

## How to Build 101
Source to object:
```
aarch64-linux-gnu-gcc -c a.S
```
`a.o` generated.

Object to ELF:
```
aarch64-linux-gnu-ld -T linker.ld -o kernel8.elf a.o
```

ELF to kernel image:
```
aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img
```

Object, ELF and Kernel Image?
- Object: 只有被編譯成 binary，有基本的 section 的資訊，沒有 symbol table，還沒被指定 logical address。
- ELF: 被 link 好了，能跑在能識別 ELF 的作業系統上。
- Kernel Image: 因為 bare metal 沒辦法讀 ELF，只保留 `.text` 跟 `.data` sections 給 QEMU 或 bootloader 處理。

## Test w/ QEMU
```
qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm
```
- `-M raspi3b`: specify 硬體。需要指定硬體，是因為硬體還包括 GPIO, CPU, memory layout 等即使是同個 CPU 架構也會有差異的部分。
- `-kernel`: 指定 kernel image
- `-display none`: 沒 GUI。
- `-d in_asm`: `-d` 後面加輸出選項，`in_asm` 是把 machine code disassemble 成 assembly。

```
qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -S -s
```
- `-S`: suspend CPU.
- `-s`: start GDB server (default port 1234).

```
# start multi-architecture gdb
gdb-multiarch
# 可能要 set architecture aarch64 來 specify 架構
(gdb) file kernel8.elf
(gdb) target remote :1234
```
- `file kernel8.elf`: load symbol table 之類的資訊來幫助 debug（像是可以用 break XXX variable or function 之類的）。如果沒下的話，就要用 address 來下斷點。

## Deploy to RPi
