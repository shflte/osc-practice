[Spec](https://nycu-caslab.github.io/OSC2024/labs/lab1.html)

Project 開始變得有點龐大了，有點不知道怎麼切入。先從 Makefile 開始看懂好了，可以看懂檔案之間的 dependency。

# Makefile
這次 Makefile 撰寫的順序：
- Variable definition
	- Compiler & tools
	- Flags
	- Target binaries, directories, source files, generated object files
- Rules
	- Default target
	- QEMU target for testing
	- Clean
	- Rules to build the default target (in reverse order)
- .PHONY

這邊描述一些幫助理解這次的 Makefile 的東東。
常見的怪怪符號：
- `$<`: 第一個 dependency。
- `$^`: 所有 dependency。
- `$@`: 這條 rule 的 target。

常見的 assignment：
- `=`: Lazy assignment. 在被使用的時候才會展開。
- `:=`: Immediate assignment. Variable value 馬上就會被固定。
- `?=`: Conditional assignment. 只有在 variable 沒 value 或是為空時才會做 assignment。

Etc:
- `$(X:Y=Z)`: 把 `X` 中 match pattern `Y` 的都替換成 `Z`。`X` 可以是個 string, wildcard pattern 或是 variable；`Y` 要寫每個要替換的對象的 pattern。`Z` 是要替換成什麼。
- `$(wildcard Pattern)`: 把 match 右邊 `Pattern` 的所有東西用空格 concat 起來。
- `$(patsubst Y, Z, X)`: 類似 `$(X:Y=Z)`。
- `target: dependency | sth`: 這裡的 `sth` 是 order-only dependency，放在 `|` 右邊。Order-only dependency 放一些在 build 這個 target 時一定要存在但只需要存在即可的 dependency。這種 dependency 不會在被更新時 trigger target 的重新編譯。
- `.PHONY`: Phony 的意思是假的，在 Makefile 中的用途就是定義假的 targets。使用時機是某些 target 總是需要被處理。

# Linker Script
Linker script 的目的：
- 把 object files 串起來。
- 決定每個 section 分別在 runtime 時應該被 load 到的 logical address。

### Section Mapping
```
<output section> : {<object files>(<input sections>)}
```
把 object files 的 input sections 都放進 output section 中。Output section 和 input sections 不需要一樣。可以用 `*` 來 match 所有 object files。
Ex.:
- `.text.boot : { *(.text.boot) }`
- `.haha : {a.o(.data)}`

### Memory Address Management
```
. = <mem addr>;
```
- 把當下的 memory address 移到 `mem addr` 這裡
- `ALIGN(0xN)`: 下一個對其 `N` 個 bytes 的倍數的 memory address。

### Address Assignment
```
sth = .;
```
- 把這裡的 memory address assign 給 symbol `sth`。

### 這次有用到的 Section 朋友
- `.text`: 所有 function 跟 code。
- `.rodata`: `ro` for read-only，這裡放用 `const` 宣告的東西。
- `.data`: initialized 的 global 或 static variables。
- `.bss`uninitialized 的 global 或 static variables。
	- 在 Linux 之類的環境中，通常會是 loader 或是 runtime 的 C library 在處理把 `.bss` section 清零的步驟。但這裡 Bootloader 是自己寫的，所以要自己清零。
	- 可以像這次用 `bss_begin` 和 `bss_end` 兩個 symbol 來記錄位置，ARM GNU tool chain 也是用開頭跟結尾的 symbol 來清零 `.bss` 的；也可以用開頭的位置 + size。
- `.text.boot`: 通常放在開頭，因為 RPi 的 Bootloader 預設從記憶體位置開頭開始執行。

# C & Assembly Explained
整個 kernel 的執行順序大概是：
- 在 `Boot.S` 中把 CPU 和 memory 大概 setup 好，然後去 `kernel_main`。
- 在 `kernel_main` call `uart_init()` 中把 UART setup 好。
- `shell()`

跟 peripherals 交互的過程大概就是往某些 mapped 到 peripherals 的 registers 的 memory address 寫東西，然後去讀另個特定的 memory address 看東西好了沒。如果有用到 GPIO 的話，還要先做 GPIO 的 alternative function selection，然後處理 GPIO 的 pull-up/pull-down。

這邊除了看懂每一行在幹嘛之外，還要知道這些資訊從手冊上哪裡找到的。

`Boot.S`
內容都定義在 `.text.boot` section。
在把控制跳去 `kernel_main` 之前，主要完成這幾件事：
- 讓其他 CPU 去發呆。
- 清零 `.bss`。
- 設好 stack pointer。

`memzero.S`
把 `x0` 開始，長度總共 `x1` bytes 的這段記憶體逐 8 byte 清零。

`utils.S`
- `put32`, `get32`: 就記憶體跟 register 傳東西。
- `delay`: delay...

`include/peripheral/XX.h`
定義各種 register 被 mapped 到的 memory address 等。
- `base.h`: 所有 peripherals 的 base address。
- `mini_uart.h`: 手冊 P.8。
- `mail_box.h`: 鑽牛角尖老半天才發現 lab1 spec 有寫 == 。掙扎的過程放下面。
- `power_management.h`: 也是 lab1 spec 有寫。
- `GPIO.h`: 手冊 P.90。

# Reference
## Assembly 大補帖
#### Register 朋友
- `xi`: General purpose register. 前四個參數預設用 `x0`-`x3` 來傳。
- `wi`: 對應 `xi` 的後半 32 bits。
- `[xi]`: `xi` register 存著的 mem addr 指向的記憶體位置。

#### Instruction 朋友
- `mrs`: Move to general purpose Register to System register. 把 system register 的內容 load 到 general purpose register。
- `cbz`: Compare register value to Zero and Branch to the target symbol.
- `adr`: 把 target symbol 相對 PC 的 address load 到 register 中。尾端的 `#8` 表示 `x0` 會自動被 increment 8 bytes。
- `str`: 把 register 中的值 store 到 mem addr `[xi]` 中。
- `ldr`: 把 mem addr `[xi]` 的值 load 到 register 中。
- `xzr`: 固定是 0 的 register。
- `b.gt`: Branch on greater than. 如果上一條 instruction 的結果 greater than 0 就 branch。
- `ret`: return 到 caller。

## 怎麼多一個怪 Mapping？
上述 GPU 或 VideoCore 雖然一開始設計的目的是要處理 graphic，但現在更像 peripherals 的管理器。

Broadcom 板子手冊的 P.5-6 有解釋到 ARM 的 physical address 跟 VC CPU bus address 還有一層 VC (VideoCore) MMU 的 mapping。手冊接下來提到的都是 VC CPU bus address，peripherals 的 base address 是 `0x7E000000`；但 ARM 這邊在用的是 physical address，peripherals 的 base address 是 `0x3F000000`。有這樣的 mapping 的原因是 VC 的 peripherals address 可以固定，這樣儘管 ARM 那邊想要改 peripherals 的 memory address mapping 了，VC 的實現還是可以復用。

好我其實也還是不太能接受怎麼多一層 mapping，但隨便啦。

## 重要的文件們
[BCM2837 ARM Peripherals manual](https://github.com/raspberrypi/documentation/files/1888662/BCM2837-ARM-Peripherals.-.Revised.-.V2-1.pdf)
[Building an Operating System for the Raspberry Pi - The Mailbox Peripheral](https://jsandler18.github.io/extra/mailbox.html)
[RPi Firmware Wiki](https://github.com/raspberrypi/firmware/wiki)
- 找不到 mailbox 的 bus address 還有 registers 寫在手冊上的什麼地方。邏輯上 mailbox 的 address 應該是 Broadcom 要訂定的，因為他們的 VideoCore 是用這個 address 去存取 mailbox 的，但我只在 [RPi Firmware Wiki](https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes) 這裡看到 `0xB880`。
- [Mailbox property interface](https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface) 有放各種 ARM to VC 的 tag 和 format。