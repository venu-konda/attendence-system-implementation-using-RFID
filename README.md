# RFID attendance system (NXP LPC2129)

Firmware and host-side tooling for an RFID-based attendance logger built on an
NXP LPC2129 (ARM7TDMI) development board, written in C for Keil µVision. A
tag presented to the RFID reader is matched against a small list of known
people, the person's IN/OUT state is toggled and remembered in an external
I²C EEPROM, the event is shown on a 20×4 character LCD, and a CSV record with
the on-chip RTC time and date is sent over UART0 to a PC. A small Linux
program on the PC captures those records into a spreadsheet-readable file.

## Repository contents

All files live in `project_new/`, the folder layout the project was developed
in. The firmware sources, Keil project files, flashable `.hex` images,
host-side logger sources and sample log files are committed; Keil's
intermediate build outputs are not (see [Build outputs](#build-outputs)).
The project was originally uploaded as a single zip archive; it was unpacked
into this layout so the code is browsable and diffable.

### Keil projects

| Project file | Sources it builds | Purpose |
|---|---|---|
| `complete.uvproj` | `complete_file.c`, `delay.c`, `i2c_new.c`, `i2c_eeprom_new.c`, `interrupt.c`, `kpm.c`, `lcd.c`, `Startup.s` | **The final attendance firmware.** Its output image is `complete.hex`. |
| `i2c.uvproj` | `i2c_main.c`, `i2c.c`, `i2c_eeprom.c`, `delay.c`, `Startup.s` | Stand-alone I²C EEPROM write/read test (lights a green or red LED on P1.16/P1.17). |
| `rtc.uvproj` | `rtc_main.c`, `uart0.c`, `external_interrupts_test.c`, `delay.c`, `kpm.c`, `Startup.s` | RTC test: prints time and date over UART every second, with keypad-driven time setting on an external interrupt. |
| `uart_interupt.uvproj` | `uart_int.c`, `Startup.s` | UART0 receive-interrupt test: echoes the received tag ID and prints `IN` if it matches a hard-coded list. |
| `comp.uvproj` | (empty file) | Leftover, 0 bytes. |

Target settings in the projects: device LPC2129, 12 MHz crystal, CPU clock
60 MHz (PLL ×5), peripheral clock 15 MHz.

### Firmware source files

| File | Role |
|---|---|
| `complete_file.c` | Main program for the attendance system: UART0 driver with receive interrupt, RTC setup and printing, LCD time display, and `main()` |
| `interrupt.c` | External-interrupt handler that shows a keypad-driven menu on the LCD for setting hour, minute, second, date, month, year and weekday |
| `lcd.c`, `lcd.h`, `lcd_defines.h` | HD44780 character LCD driver, 8-bit mode (data on P1.24–P1.31, RS on P0.21, RW on P0.20, EN on P0.22) |
| `kpm.c`, `kpm.h`, `kpm_defines.h` | 4×4 matrix keypad driver (rows P1.16–P1.19, columns P1.20–P1.23) |
| `i2c_new.c`, `i2c.h`, `i2c_defines.h` | I²C master driver on the LPC2129 I²C peripheral at 100 kHz |
| `i2c_eeprom_new.c`, `i2c_eeprom.h` | Byte and page read/write for a 24-series I²C EEPROM at slave address 0x50 |
| `delay.c`, `delay.h` | Busy-wait delays in µs, ms and s, calibrated for the 60 MHz clock |
| `defines.h`, `pin_function_defines.h`, `types.h` | Bit-manipulation macros, PINSEL configuration macro, fixed-width type aliases |
| `Startup.s` | Keil startup code for the LPC21xx |
| `i2c.c`, `i2c_eeprom.c`, `i2c_definess.h`, `i2c_main.c` | Earlier versions of the I²C files, used by the `i2c` test project |
| `uart0.c`, `uart0.h`, `rtc_main.c`, `external_interrupts_test.c` | Files of the `rtc` test project |
| `uart_int.c` | Single-file `uart_interupt` test project |

### Host-side (Linux PC) files

| File | Role |
|---|---|
| `xl.c`, `xl.h` | Serial-port helpers (`serialOpen`, `serialGetchar`, ...) using termios; adapted from the wiringPi serial code |
| `xl_test.c` | Logger `main()`: opens `/dev/ttyUSB0` at 9600 baud, echoes every received character, and hands each complete line to `writefile()` |
| `write.c`, `write1.c` | Two variants of `writefile()`, which appends a line to `data1.xls` |
| `write_menu.c` | Writes a `s.no,name,id,status,time` header row to `data.xls` (not called in the current logger) |
| `write2.c` | Stand-alone test that appends one fixed record to `d.xls` |
| `data1.xls`, `d.xls`, `data` | Sample captured logs. Despite the extension these are plain CSV text, not Excel binaries; they open in Excel or LibreOffice as CSV |

### Build outputs

The five `*.hex` files are the flashable images Keil produced for each
project; `complete.hex` is the one for the final firmware. The `*.sct`
scatter files are kept because the projects reference them. Everything else
Keil generates (`*.o`, `*.crf`, `*.d`, `*.axf`, `*.map`, `*.htm`, `*.lnp`,
`*.plg`, `*.tra`, `*.dep`, `*.lst`, `*.__i`, `*.bak`, `*.uvopt`) and the
compiled host logger are excluded by `.gitignore` and come back on a rebuild.

## How the attendance firmware works

1. **Startup** (`main()` in `complete_file.c`): initialises UART0 at 9600
   baud 8N1 with the receive interrupt enabled, the I²C bus, the on-chip RTC
   (preset to 15/05/24 09:59:57, Friday), the LCD, and an external interrupt
   for the time-setting button. The LCD shows `WAITING FOR INPUT:`.
2. **Tag read**: the RFID reader is wired to UART0. The receive interrupt
   accumulates characters into a buffer until it sees an ETX byte (0x03),
   then sets a flag that wakes the main loop.
3. **Lookup**: the received string is compared (substring match) against
   three hard-coded tag IDs with their names:

   | Name | Tag ID |
   |---|---|
   | kala | 12600717 |
   | harsha | 00326553 |
   | sri | 00354554 |

4. **IN/OUT state**: for a matched person, one byte at EEPROM address
   `0x0000 + index` holds their current state (0 = out, 1 = in). The firmware
   reads it, treats any value other than 0 or 1 as 0, reports the opposite
   event, and writes the new state back, so the state survives a reset.
5. **Output**: a CSV record is sent over UART0 and the name, event, time and
   date are shown on the LCD for one second:

   ```
   harsha,"00326553",IN,10:03:00,15/05/24,SUNDAY
   ```

   An unknown tag shows `invalid` on the LCD and logs nothing.
6. **Setting the clock**: pressing the button on P0.7 raises an external
   interrupt (EINT2; the code names the function `Enable_EINT0` and the
   handler `eint1_isr`, but it configures EINT2). The handler runs a menu on
   the LCD, `1.HOUR 2.MIN 3.SEC 4.DATE 5.MONTH 6.YEAR 7.DAY`, and reads
   numbers from the 4×4 keypad. Digit keys build the number; any non-digit key
   (`+ - * / = c`) ends the entry. Out-of-range values show `INVALID INPUT`
   and return to the menu. The whole menu runs inside the interrupt handler,
   so tag reads are not serviced until it finishes.

The host logger (`xl_test.c`) reads the same serial line on the PC and appends
every received line to `data1.xls`, so the file accumulates one row per tag
event. `data1.xls` in the zip is a real capture from a test session.

## Building

### Firmware

1. Open `project_new/complete.uvproj` in Keil µVision 4 or 5 with the Legacy
   ARM7/ARM9 device support (ARMCC compiler) installed.
2. Build (F7). The output is `complete.hex`.
3. Flash with Flash Magic or any LPC2000 ISP tool over the board's serial
   port, or with a JTAG debugger.

The other `.uvproj` files build the stand-alone peripheral tests the same way.

### Host logger (Linux)

```sh
cd project_new
gcc xl_test.c xl.c write1.c -o logger
./logger                # reads /dev/ttyUSB0 at 9600 baud, appends to data1.xls
```

Use `write.c` instead of `write1.c` for the variant that appends a comma
after each line. Change the device path in `xl_test.c` if the USB-serial
adapter enumerates differently. `xl.h` declares `serialGetchar` as returning
`int` while `xl.c` defines it returning `char`; it links because `xl.c` does
not include the header, but the mismatch is worth fixing.

## Things to know

- Only three people are recognised and their IDs and names are compiled in.
  To add someone, extend the `id` and `name` arrays in `complete_file.c` and
  rebuild; the EEPROM state byte index follows the array index.
- The UART receive buffer is 10 bytes, so a tag string of 10 or more
  characters before the ETX overflows it.
- The RTC starting time is hard-coded; set the real time with the keypad menu
  after each power-up, or keep a backup battery on VBAT.
- `comp.uvproj` is empty and can be deleted.

## License

MIT. See [LICENSE](LICENSE).
