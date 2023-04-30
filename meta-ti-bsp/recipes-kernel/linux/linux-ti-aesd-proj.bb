SECTION = "kernel"
SUMMARY = "Linux kernel for TI devices"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"


LINUX_VERSION ?= "5.10"
inherit kernel
require recipes-kernel/linux/linux-yocto.inc

DEPENDS += "gmp-native libmpc-native"
KEEPUIMAGE  = "no"
COMPRESS_IMAGE = "none"

# Look in the generic major.minor directory for files
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}-5.10:"

KERNEL_DEFCONFIG = "aesd_defconfig"
KERNEL_DEVICETREE:append = "da850-lcdk_aesd.dtb"

PV = "5.10.177+git${SRCPV}"

BRANCH = "main"
KERNEL_GIT_URI = "git://git@github.com/sujoyray-ucolorado/final-project-assignment-sujoyray.git"
KERNEL_GIT_PROTOCOL= "https"
SRCREV = "624b6f1202fdc6182ecbc41026c185c3f091e428"

SRC_URI += "${KERNEL_GIT_URI};protocol=${KERNEL_GIT_PROTOCOL};branch=${BRANCH} \
            file://aesd_defconfig"

FILES:${KERNEL_PACKAGE_NAME}-devicetree += "/${KERNEL_IMAGEDEST}/*.itb"

## The following recipe was used from https://gist.github.com/rgov/0628785685ab858a99c5bfca626c1d8f

uboot_prep_kimage:append () {
    # This function is defined in kernel-devicetree.class
    dtb_file=`get_real_dtb_path_in_kernel "${KERNEL_DEVICETREE}"`

    mv linux.bin linux-orig.bin
    cat linux-orig.bin "${dtb_file}" > linux+dtb.bin
    ln -s linux+dtb.bin linux.bin
}
