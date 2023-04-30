require conf/machine/include/ti-soc.inc
SOC_FAMILY:append = ":omapl138_aesd_proj"

require conf/machine/include/arm/armv5/tune-arm926ejs.inc


UBOOT_ENTRYPOINT = "0xc0008000"
UBOOT_LOADADDRESS = "0xc0008000"
UIMAGE_LOADADDR = "0xc0008000"
KERNEL_IMAGE_COMPRESSION = ""
KERNEL_COMPRESSION_LEVEL = "0"

KERNEL_IMAGETYPES = "zImage uImage"
KERNEL_CLASSES += "kernel-uimage"
#KERNEL_DEVICETREE_BUNDLE = "1"





#PREFERRED_PROVIDER_virtual/kernel ?= "linux-ti-staging"
PREFERRED_PROVIDER_virtual/kernel ?= "linux-ti-aesd-proj"
#PREFERRED_PROVIDER_virtual/bootloader = "u-boot-ti-staging"
#PREFERRED_PROVIDER_u-boot = "u-boot-ti-staging"
PREFERRED_PROVIDER_virtual/bootloader = "uboot-aesd-proj"
PREFERRED_PROVIDER_u-boot = "uboot-aesd-proj"

EXTRA_IMAGEDEPENDS += "virtual/bootloader"

MACHINE_FEATURES = "kernel26 serial ethernet usbhost usbgadget mmc alsa"
