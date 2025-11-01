# embedded-smartPDB
> A clean, no-nonsense STM32 project setup using PlatformIO, STM32Cube HAL, and FreeRTOS. Because life's too short for bad embedded setups.

![CI Status](https://github.com/CPRT/embedded-smartPDB/workflows/Full%20CI%20Check/badge.svg)

## What You're Getting

- **STM32F103C8**
- **PlatformIO**
- **STM32Cube HAL** 
- **FreeRTOS**

## Quick Start

### 1. Prerequisites

You'll need:
- VS Code
- PlatformIO extension
- ST-Link V2 (or compatible programmer)
- A sense of whismy and adventure

```bash
# if you are on linux you might need this to give permissions to use the USB ports for flashing
sudo usermod -a -G dialout $USER
# Then logout/login
```

### 2. Clone & Setup

```bash
git clone git@github.com:CPRT/embedded-smartPDB.git
cd embedded-smartPDB
code .
```

VS Code will bug you about installing extensions. **Click "Install All"** 


### 3. Build It

```bash
# Generate compile_commands.json for linting
pio run -t compiledb
ln -s .pio/build/bluepill_f103c8/compile_commands.json compile_commands.json

# Build the firmware
pio run
```

If it compiles, you're golden. ✨

### 4. Flash It

```bash
# Upload to board
pio run -t upload

# Watch serial output (optional)
pio device monitor
```

## Project Structure

```
.
├── include/              # Your headers go here
│   ├── FreeRTOSConfig.h # RTOS configuration
│   └── main.h           # Main header
├── src/                  # Your code lives here
│   ├── main.c           # Entry point
│   └── freertos.c       # RTOS tasks
├── lib/                  # Local libraries
│
├── platformio.ini       # Project config
└── .vscode/             # VS Code goodies
```

## Common Tasks

### Build
```bash
pio run
# Or in VS Code: Ctrl+Shift+B
```

### Upload
```bash
pio run -t upload
```

### Clean Build
```bash
pio run -t clean && pio run
```

### Monitor Serial
```bash
pio device monitor
# Exit with Ctrl+C
```

### Static Analysis
```bash
pio check
```

### Memory Report
```bash
arm-none-eabi-size -A -d .pio/build/bluepill_f103c8/firmware.elf
```


## Adding FreeRTOS Tasks

In  `src/freertos.c`:

```c
void MyTask(void *pvParameters) {
    while(1) {
        // Do stuff
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay 1 second
    }
}

// In main()
xTaskCreate(MyTask, "MyTask", 128, NULL, 1, NULL);
vTaskStartScheduler();
```

## Debugging

Got an ST-Link? Time to debug properly.

### Quick Debug
```bash
# In VS Code: F5
# Or: Run → Start Debugging
```

Uses the config in `.vscode/launch.json` (ST-Link by default).

### Manual GDB Session
```bash
# Terminal 1: Start OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

# Terminal 2: GDB
arm-none-eabi-gdb .pio/build/bluepill_f103c8/firmware.elf
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue
```

## Resources

### Documentation
- [PlatformIO Docs](https://docs.platformio.org/)
- [STM32F103 Reference Manual](https://www.st.com/resource/en/reference_manual/cd00171190.pdf)
- [FreeRTOS Docs](https://www.freertos.org/Documentation/RTOS_book.html)

### Useful Commands
```bash
# List all tasks
pio run --list-targets

# Verbose build
pio run -v

# Clean everything
rm -rf .pio

# Check library dependencies
pio lib list
```

## Contributing

1. Don't break the build
2. Test your changes
3. Update this README if you add something cool
4. Keep commits atomic and meaningful