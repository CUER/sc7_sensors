# Drivers for SC7 sensors

## Building the code

### Dependencies:
- [arm-none-eabi-gcc](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
- [make](https://www.gnu.org/software/make/) (and for [Windows](https://gnuwin32.sourceforge.net/packages/make.htm))
- [openocd](https://openocd.org/)
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) (Optional - only needed to configure pins)

A [guide](https://www.youtube.com/watch?v=PxQw5_7yI8Q) for setup on windows.

### Compiling and flashing:

```bash
make flash
```

## IMU - LS6DS032

[Datasheet](https://www.mouser.co.uk/pdfDocs/STM_LSM6DSO32_DS.pdf)

Driver is based on [this](https://github.com/adafruit/Adafruit_LSM6DS) Adafruit library.

## Temp sensor - MCP9808

[Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/25095A.pdf)

Driver is based on [this](https://github.com/adafruit/Adafruit_MCP9808_Library) Adafruit library.

## Temp/pressure/humidity sensor - BME280

[Datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf)

Driver is based on [this](https://github.com/adafruit/Adafruit_BME280_Library) Adafruit library.