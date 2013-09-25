#---------------------------------------------------------------------------
# MAKE 파일
#
# 수정 : 
#
# 날짜 : 2013. 8. 19.
#
# 
#---------------------------------------------------------------------------



#---------------------------------------------------------------------------
# Toolchain prefix (i.e arm-elf- -> arm-elf-gcc.exe)
#---------------------------------------------------------------------------
TCHAIN_PREFIX = arm-none-eabi-
REMOVE_CMD=cs-rm


#---------------------------------------------------------------------------
# Startup 파일 
#---------------------------------------------------------------------------
STARTUP_C = Hw_StartUp.c


#---------------------------------------------------------------------------
# 타켓 이미지
#---------------------------------------------------------------------------
TARGET = main


#---------------------------------------------------------------------------
# 빌드 결과 파일 저장 디렉토리
#---------------------------------------------------------------------------
OUTDIR = ./Build


#---------------------------------------------------------------------------
# 추가 헤더 파일이 속해 있는 디렉토리 
# -각 디렉토리는 공백으로 구분
#---------------------------------------------------------------------------
EXTRAINCDIRS = ../LIB_AP ../LIB_LB ../LIB_HW ../ ../LIB_USB ../LIB_USB/DRV


#---------------------------------------------------------------------------
# 소스 파일
#---------------------------------------------------------------------------

SRC_AP = 	../main.c		\
			../LIB_AP/Ap.c	\
			../LIB_AP/Ap_RadioMenu.c	\
			../LIB_AP/Ap_Radio.c

SRC_LB = 	../LIB_LB/Lb_Util.c	\
			../LIB_LB/Lb_Printf.c

SRC_HW = 	../LIB_HW/$(STARTUP_C)	\
			../LIB_HW/Hw.c			\
			../LIB_HW/Hw_Uart.c		\
			../LIB_HW/Hw_PLL.c		\
			../LIB_HW/Hw_Led.c		\
			../LIB_HW/Hw_Uart_Q.c	\
			../LIB_HW/Hw_ISR.c		\
			../LIB_HW/Hw_Timer.c	\
			../LIB_HW/Hw_VCom.c		\
			../LIB_HW/Hw_I2C.c		\
			../LIB_HW/Hw_I2C_SW.c	\
			../LIB_HW/Hw_Si47xx.c	
			
			
SRC_USB =   ../LIB_USB/usb_desc.c	\
			../LIB_USB/usb_endp.c	\
			../LIB_USB/usb_istr.c	\
			../LIB_USB/usb_prop.c	\
			../LIB_USB/usb_pwr.c	\
			../LIB_USB/hw_config.c	\
			../LIB_USB/DRV/usb_core.c	\
			../LIB_USB/DRV/usb_init.c	\
			../LIB_USB/DRV/usb_mem.c	\
			../LIB_USB/DRV/usb_regs.c	\
			../LIB_USB/DRV/usb_sil.c	\
			../LIB_USB/DRV/usb_int.c	
			
			
			
			
SRC = $(SRC_AP) $(SRC_LB) $(SRC_HW) $(SRC_USB)


#---------------------------------------------------------------------------
# THUMB 모드 옵션
#---------------------------------------------------------------------------
THUMB = -mthumb


#---------------------------------------------------------------------------
# Include 디렉토리
#---------------------------------------------------------------------------
INCLUDE = -I./include -I.


#---------------------------------------------------------------------------
# C 컴파일 옵션
#---------------------------------------------------------------------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#
# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS  = -gdwarf-2 -O2 -mcpu=cortex-m3 -mthumb-interwork -DSTM32F10X_MD 
CFLAGS += $(INCLUDE) -mlong-calls -ffunction-sections -fdata-sections -fpromote-loop-indices -Wall -Wextra -Wimplicit -Wcast-align -Wpointer-arith -Wredundant-decls 
CFLAGS += -Wshadow -Wcast-qual -Wcast-align -Wa,-adhlns=start_stm32f10x.lst -MD -MP -MF $(TARGET).elf.d
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

CONLYFLAGS += -Wnested-externs 
CONLYFLAGS += -std=gnu99

LDFLAGS = -static -nostdlib -nostartfiles -Wl,-Map=$(TARGET).map,--cref,--gc-sections -L./ -lc  -lm  -lc -lgcc -T.//STM32F10x_128k_20k_flash.ld


# Define programs and commands.
CC      = $(TCHAIN_PREFIX)gcc
CPP     = $(TCHAIN_PREFIX)g++
AR      = $(TCHAIN_PREFIX)ar
OBJCOPY = $(TCHAIN_PREFIX)objcopy
OBJDUMP = $(TCHAIN_PREFIX)objdump
SIZE    = $(TCHAIN_PREFIX)size
NM      = $(TCHAIN_PREFIX)nm
REMOVE  = $(REMOVE_CMD) -f
SHELL   = sh
###COPY    = cp


# Define Messages
# English
MSG_LOAD_FILE = Creating load file:
MSG_EXTENDED_LISTING = Creating Extended Listing/Disassembly:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = ---- Linking :
MSG_CLEANING = Cleaning project:
MSG_ASMFROMC = "Creating asm-File from C-Source:"

# List of all source files.
ALLSRC     =  $(SRC)
# List of all source files without directory and file-extension.
ALLSRCBASE = $(notdir $(basename $(ALLSRC)))

# Define all object files.
#ALLOBJ = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRCBASE)))
ALLOBJ = $(addsuffix .o, $(ALLSRCBASE))


# Default target.
all: createdirs build

elf: $(TARGET).elf
lss: $(TARGET).lss 
sym: $(TARGET).sym
hex: $(TARGET).hex
bin: $(TARGET).bin


build: elf hex bin lss sym

# Create output directories.
createdirs:
	-@md $(OUTDIR) >NUL 2>&1 || echo "" >NUL
	-@md $(OUTDIR)\dep >NUL 2>&1 || echo "" >NUL

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo $(MSG_LOAD_FILE) $@
	$(OBJCOPY) -O ihex $< $@
	
# Create final output file (.bin) from ELF output file.
%.bin: %.elf
	@echo $(MSG_LOAD_FILE) $@
	$(OBJCOPY) -O binary $< $@

# Create extended listing file/disassambly from ELF output file.
# using objdump testing: option -C
%.lss: %.elf
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -C -r $< > $@
#	$(OBJDUMP) -x -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(ALLOBJ)
%.elf:  $(ALLOBJ)
	@echo $(MSG_LINKING) $@
	$(CC) $(THUMB) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
define COMPILE_C_TEMPLATE
$(notdir $(basename $(1))).o : $(1)
	@echo $(MSG_COMPILING) $$< to $$@
	$(CC) -c $(THUMB) $$(CFLAGS) $$(CONLYFLAGS) $$< -o $$@ 
endef
$(foreach src, $(SRC), $(eval $(call COMPILE_C_TEMPLATE, $(src)))) 


# Compile: create assembler files from C source files. ARM/Thumb
$(SRC:.c=.s) : %.s : %.c
	@echo $(MSG_ASMFROMC) $< to $@
	$(CC) $(THUMB) -S $(CFLAGS) $(CONLYFLAGS) $< -o $@

###################################################################################################
###################################################################################################
###################################################################################################
###################################################################################################
###################################################################################################
# Define all listing files (used for make clean).
LSTFILES   = $(addsuffix .lst, $(ALLSRCBASE))
# Define all depedency-files (used for make clean).
DEPFILES   = $(addsuffix .o.d, $(ALLSRCBASE))


# Target: clean project.
clean: clean_list

clean_list :
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).elf.d
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(ALLOBJ)
	$(REMOVE) $(LSTFILES)
	$(REMOVE) $(DEPFILES)
	$(REMOVE) $(SRC:.c=.s)

# Create object files directory
##$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
#-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)
-include $(wildcard dep/*)

# Listing of phony targets.
.PHONY : all finish \
build elf hex bin lss sym clean clean_list createdirs

