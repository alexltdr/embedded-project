PROJECT   = reaction-time

CC        = arm-none-eabi-gcc
OBJCOPY   = arm-none-eabi-objcopy
SIZE      = arm-none-eabi-size

CPU       = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
DEFS      = -DSTM32F401xE
INCS      = -Icmsis

CFLAGS    = $(CPU) $(DEFS) $(INCS) -Wall -O2 -ffreestanding \
            -fdata-sections -ffunction-sections

LDSCRIPT  = STM32F401RETX_FLASH.ld
LDFLAGS   = $(CPU) -T$(LDSCRIPT) --specs=nosys.specs --specs=nano.specs \
            -Wl,--gc-sections -Wl,-Map=$(PROJECT).map
SRCS_C    = main.c system_stm32f4xx.c uart.c
SRCS_S    = startup_stm32f401xe.s
OBJS      = $(SRCS_C:.c=.o) $(SRCS_S:.s=.o)

all: $(PROJECT).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(PROJECT).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x8000000

clean:
	rm -f *.o *.elf *.bin *.map

.PHONY: all flash clean
