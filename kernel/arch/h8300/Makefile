platform-$(CONFIG_CPU_H8300H)	:= h8300h
platform-$(CONFIG_CPU_H8S)	:= h8s
PLATFORM := $(platform-y)

board-$(CONFIG_H8300H_GENERIC)		:= generic
board-$(CONFIG_H8300H_AKI3068NET)	:= aki3068net
board-$(CONFIG_H8300H_H8MAX)		:= h8max
board-$(CONFIG_H8300H_SIM)		:= generic
board-$(CONFIG_H8S_GENERIC)		:= generic
board-$(CONFIG_H8S_EDOSK2674)		:= edosk2674
board-$(CONFIG_H8S_SIM)			:= generic
BOARD := $(board-y)

model-$(CONFIG_RAMKERNEL)	:= ram
model-$(CONFIG_ROMKERNEL)	:= rom
MODEL := $(model-y)

cflags-$(CONFIG_CPU_H8300H)	:= -mh
ldflags-$(CONFIG_CPU_H8300H)	:= -mh8300helf
cflags-$(CONFIG_CPU_H8S)	:= -ms
ldflags-$(CONFIG_CPU_H8S)	:= -mh8300self

KBUILD_CFLAGS += $(cflags-y)
KBUILD_CFLAGS += -mint32 -fno-builtin
KBUILD_CFLAGS += -g
KBUILD_CFLAGS += -D__linux__
KBUILD_CFLAGS += -DUTS_SYSNAME=\"uClinux\"
KBUILD_AFLAGS += -DPLATFORM=$(PLATFORM) -DMODEL=$(MODEL) $(cflags-y)
LDFLAGS += $(ldflags-y)

CROSS_COMPILE = h8300-elf-
LIBGCC := $(shell $(CROSS-COMPILE)$(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

head-y := arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)/crt0_$(MODEL).o

core-y	+= arch/$(ARCH)/kernel/ \
	   arch/$(ARCH)/mm/
ifdef PLATFORM
core-y	+= arch/$(ARCH)/platform/$(PLATFORM)/ \
	   arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)/
endif

libs-y	+= arch/$(ARCH)/lib/ $(LIBGCC)

boot := arch/h8300/boot

export MODEL PLATFORM BOARD

archmrproper:

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

vmlinux.srec vmlinux.bin zImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

define archhelp
  @echo  'vmlinux.bin  - Create raw binary'
  @echo  'vmlinux.srec - Create srec binary'
  @echo  'zImage       - Compressed kernel image'
endef
