# UBOOT_LOCALVERSION can be set to add a tag to the end of the
# U-boot version string.  such as the commit id
UBOOT_LOCALVERSION = "-g${@d.getVar('SRCPV', True).replace('AUTOINC+','')[:10]}"

UBOOT_SUFFIX ?= "img"
SPL_BINARY ?= "MLO"

require ${COREBASE}/meta/recipes-bsp/u-boot/u-boot-common.inc
require ${COREBASE}/meta/recipes-bsp/u-boot/u-boot.inc

FILESEXTRAPATHS:prepend := "${THISDIR}/u-boot:"

SUMMARY = "u-boot bootloader for TI devices"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

BRANCH ?= "master"
UBOOT_GIT_URI = "git://git.ti.com/git/ti-u-boot/ti-u-boot.git"
UBOOT_GIT_PROTOCOL = "https"
SRC_URI = "${UBOOT_GIT_URI};protocol=${UBOOT_GIT_PROTOCOL};branch=${BRANCH}"

PV:append = "+git${SRCPV}"

# u-boot needs devtree compiler to parse dts files
DEPENDS += "dtc-native bc-native flex-native bison-native python3-setuptools-native"

DM_FIRMWARE = "ipc_echo_testb_mcu1_0_release_strip.xer5f"

PLAT_SFX = ""
PLAT_SFX:j721e = "j721e"
PLAT_SFX:j7200 = "j7200"
PLAT_SFX:j721s2 = "j721s2"
PLAT_SFX:j784s4 = "j784s4"
PLAT_SFX:am65xx = "am65xx"
PLAT_SFX:am64xx = "am64xx"
PLAT_SFX:am62xx = "am62xx"
PLAT_SFX:am62axx = "am62axx"

PACKAGECONFIG[atf] = "ATF=${STAGING_DIR_HOST}/firmware/bl31.bin,,trusted-firmware-a"
PACKAGECONFIG[optee] = "TEE=${STAGING_DIR_HOST}${nonarch_base_libdir}/firmware/bl32.bin,,optee-os"
PACKAGECONFIG[dm] = "DM=${STAGING_DIR_HOST}${nonarch_base_libdir}/firmware/ti-dm/${PLAT_SFX}/${DM_FIRMWARE},,ti-rtos-firmware"

PACKAGECONFIG:append:aarch64 = " atf optee"
PACKAGECONFIG:append:j721e = " dm"
PACKAGECONFIG:append:j7200 = " dm"
PACKAGECONFIG:append:j721s2 = " dm"
PACKAGECONFIG:append:j784s4 = " dm"
PACKAGECONFIG:append:am62xx = " dm"
PACKAGECONFIG:append:am62axx = " dm"

COMPATIBLE_MACHINE = "(ti-soc)"

EXTRA_OEMAKE += "${PACKAGECONFIG_CONFARGS}"

PROVIDES += "u-boot"
PKG:${PN} = "u-boot"
PKG:${PN}-dev = "u-boot-dev"
PKG:${PN}-dbg = "u-boot-dbg"

S = "${WORKDIR}/git"

# Support for secure devices - detailed info is in doc/README.ti-secure
inherit ti-secdev

SYSROOT_DIRS += "/boot"

# SPL (Second Program Loader) to be loaded over UART
SPL_UART_BINARY = "u-boot-spl.bin"
SPL_UART_BINARY:k3r5 = ""

SPL_UART_IMAGE ?= "${SPL_UART_BINARY}-${MACHINE}-${PV}-${PR}"
SPL_UART_SYMLINK ?= "${SPL_UART_BINARY}-${MACHINE}"

# SPI NOR Flash binaries
UBOOT_SPI_SPL_BINARY = "u-boot-spl.bin"
UBOOT_SPI_BINARY = "u-boot.img"
UBOOT_SPI_GPH_BINARY = "u-boot-spi.gph"

# SPI NOR Flash deployed images
UBOOT_SPI_SPL_IMAGE = "u-boot-spl-${MACHINE}-${PV}-${PR}.bin"
UBOOT_SPI_SPL_SYMLINK = "u-boot-spl-${MACHINE}.bin"
UBOOT_SPI_IMAGE = "u-boot-${MACHINE}-${PV}-${PR}.img"
UBOOT_SPI_SYMLINK = "u-boot-${MACHINE}.img"
UBOOT_SPI_GPH_IMAGE = "u-boot-spi-${MACHINE}-${PV}-${PR}.gph"
UBOOT_SPI_GPH_SYMLINK = "u-boot-spi-${MACHINE}.gph"

# HS XLD
UBOOT_HS_XLD_BINARY = "u-boot-spl_HS_X-LOADER"
UBOOT_HS_XLD_IMAGE = "u-boot-spl_HS_X-LOADER-${MACHINE}-${PV}-${PR}"
UBOOT_HS_XLD_SYMLINK = "u-boot-spl_HS_X-LOADER-${MACHINE}"

# HS MLO
UBOOT_HS_MLO_BINARY = "u-boot_HS_MLO"
UBOOT_HS_MLO_IMAGE = "u-boot_HS_MLO-${MACHINE}-${PV}-${PR}"
UBOOT_HS_MLO_SYMLINK = "u-boot_HS_MLO-${MACHINE}"

# HS ISSW
UBOOT_HS_ISSW_BINARY = "u-boot-spl_HS_ISSW"
UBOOT_HS_ISSW_IMAGE = "u-boot-spl_HS_ISSW-${MACHINE}-${PV}-${PR}"
UBOOT_HS_ISSW_SYMLINK = "u-boot-spl_HS_ISSW-${MACHINE}"

# HS 2ND
UBOOT_HS_2ND_BINARY = "u-boot-spl_HS_2ND"
UBOOT_HS_2ND_IMAGE = "u-boot-spl_HS_2ND-${MACHINE}-${PV}-${PR}"
UBOOT_HS_2ND_SYMLINK = "u-boot-spl_HS_2ND-${MACHINE}"

do_compile:append:k3r5 () {
	if ! [ -f ${B}/${UBOOT_BINARY} ]; then
		ln -s spl/${UBOOT_BINARY} ${B}/${UBOOT_BINARY}
	fi
}

do_install:append () {
	if [ -n "${UBOOT_CONFIG}" ]
	then
		for config in ${UBOOT_MACHINE}; do
			i=$(expr $i + 1);
			for type in ${UBOOT_CONFIG}; do
				j=$(expr $j + 1);
				if [ $j -eq $i ]
				then
					if [ "x${SPL_UART_BINARY}" != "x" ]; then
						install ${B}/${config}/spl/${SPL_UART_BINARY} ${D}/boot/${SPL_UART_IMAGE}-${type}
						ln -sf ${SPL_UART_IMAGE}-${type} ${D}/boot/${SPL_UART_BINARY}-${type}
						ln -sf ${SPL_UART_IMAGE}-${type} ${D}/boot/${SPL_UART_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_XLD_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_XLD_BINARY} ${D}/boot/${UBOOT_HS_XLD_IMAGE}-${type}
						ln -sf ${UBOOT_HS_XLD_IMAGE}-${type} ${D}/boot/${UBOOT_HS_XLD_BINARY}-${type}
						ln -sf ${UBOOT_HS_XLD_IMAGE}-${type} ${D}/boot/${UBOOT_HS_XLD_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_MLO_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_MLO_BINARY} ${D}/boot/${UBOOT_HS_MLO_IMAGE}-${type}
						ln -sf ${UBOOT_HS_MLO_IMAGE}-${type} ${D}/boot/${UBOOT_HS_MLO_BINARY}-${type}
						ln -sf ${UBOOT_HS_MLO_IMAGE}-${type} ${D}/boot/${UBOOT_HS_MLO_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_ISSW_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_ISSW_BINARY} ${D}/boot/${UBOOT_HS_ISSW_IMAGE}-${type}
						ln -sf ${UBOOT_HS_ISSW_IMAGE}-${type} ${D}/boot/${UBOOT_HS_ISSW_BINARY}-${type}
						ln -sf ${UBOOT_HS_ISSW_IMAGE}-${type} ${D}/boot/${UBOOT_HS_ISSW_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_2ND_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_2ND_BINARY} ${D}/boot/${UBOOT_HS_2ND_IMAGE}-${type}
						ln -sf ${UBOOT_HS_2ND_IMAGE}-${type} ${D}/boot/${UBOOT_HS_2ND_BINARY}-${type}
						ln -sf ${UBOOT_HS_2ND_IMAGE}-${type} ${D}/boot/${UBOOT_HS_2ND_BINARY}
					fi
				fi
			done
			unset  j
		done
		unset  i
	else
		if [ "x${SPL_UART_BINARY}" != "x" ]; then
			install ${B}/spl/${SPL_UART_BINARY} ${D}/boot/${SPL_UART_IMAGE}
			ln -sf ${SPL_UART_IMAGE} ${D}/boot/${SPL_UART_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_XLD_BINARY} ]; then
			install ${B}/${UBOOT_HS_XLD_BINARY} ${D}/boot/${UBOOT_HS_XLD_IMAGE}
			ln -sf ${UBOOT_HS_XLD_IMAGE} ${D}/boot/${UBOOT_HS_XLD_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_MLO_BINARY} ]; then
			install ${B}/${UBOOT_HS_MLO_BINARY} ${D}/boot/${UBOOT_HS_MLO_IMAGE}
			ln -sf ${UBOOT_HS_MLO_IMAGE} ${D}/boot/${UBOOT_HS_MLO_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_ISSW_BINARY} ]; then
			install ${B}/${UBOOT_HS_ISSW_BINARY} ${D}/boot/${UBOOT_HS_ISSW_IMAGE}
			ln -sf ${UBOOT_HS_ISSW_IMAGE} ${D}/boot/${UBOOT_HS_ISSW_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_2ND_BINARY} ]; then
			install ${B}/${UBOOT_HS_2ND_BINARY} ${D}/boot/${UBOOT_HS_2ND_IMAGE}
			ln -sf ${UBOOT_HS_2ND_IMAGE} ${D}/boot/${UBOOT_HS_2ND_BINARY}
		fi
	fi
}

do_deploy:append () {
	if [ -n "${UBOOT_CONFIG}" ]
	then
		for config in ${UBOOT_MACHINE}; do
			i=$(expr $i + 1);
			for type in ${UBOOT_CONFIG}; do
				j=$(expr $j + 1);
				if [ $j -eq $i ]
				then
					cd ${DEPLOYDIR}
					if [ "x${SPL_UART_BINARY}" != "x" ]; then
						install ${B}/${config}/spl/${SPL_UART_BINARY} ${SPL_UART_IMAGE}-${type}
						rm -f ${SPL_UART_BINARY} ${SPL_UART_SYMLINK}
						ln -sf ${SPL_UART_IMAGE}-${type} ${SPL_UART_BINARY}-${type}
						ln -sf ${SPL_UART_IMAGE}-${type} ${SPL_UART_BINARY}
						ln -sf ${SPL_UART_IMAGE}-${type} ${SPL_UART_SYMLINK}-${type}
						ln -sf ${SPL_UART_IMAGE}-${type} ${SPL_UART_SYMLINK}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_XLD_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_XLD_BINARY} ${UBOOT_HS_XLD_IMAGE}-${type}
						rm -f ${UBOOT_HS_XLD_BINARY} ${UBOOT_HS_XLD_SYMLINK}
						ln -sf ${UBOOT_HS_XLD_IMAGE}-${type} ${UBOOT_HS_XLD_SYMLINK}-${type}
						ln -sf ${UBOOT_HS_XLD_IMAGE}-${type} ${UBOOT_HS_XLD_SYMLINK}
						ln -sf ${UBOOT_HS_XLD_IMAGE}-${type} ${UBOOT_HS_XLD_BINARY}-${type}
						ln -sf ${UBOOT_HS_XLD_IMAGE}-${type} ${UBOOT_HS_XLD_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_MLO_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_MLO_BINARY} ${UBOOT_HS_MLO_IMAGE}-${type}
						rm -f ${UBOOT_HS_MLO_BINARY} ${UBOOT_HS_MLO_SYMLINK}
						ln -sf ${UBOOT_HS_MLO_IMAGE}-${type} ${UBOOT_HS_MLO_SYMLINK}-${type}
						ln -sf ${UBOOT_HS_MLO_IMAGE}-${type} ${UBOOT_HS_MLO_SYMLINK}
						ln -sf ${UBOOT_HS_MLO_IMAGE}-${type} ${UBOOT_HS_MLO_BINARY}-${type}
						ln -sf ${UBOOT_HS_MLO_IMAGE}-${type} ${UBOOT_HS_MLO_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_ISSW_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_ISSW_BINARY} ${UBOOT_HS_ISSW_IMAGE}-${type}
						rm -f ${UBOOT_HS_ISSW_BINARY} ${UBOOT_HS_ISSW_SYMLINK}
						ln -sf ${UBOOT_HS_ISSW_IMAGE}-${type} ${UBOOT_HS_ISSW_SYMLINK}-${type}
						ln -sf ${UBOOT_HS_ISSW_IMAGE}-${type} ${UBOOT_HS_ISSW_SYMLINK}
						ln -sf ${UBOOT_HS_ISSW_IMAGE}-${type} ${UBOOT_HS_ISSW_BINARY}-${type}
						ln -sf ${UBOOT_HS_ISSW_IMAGE}-${type} ${UBOOT_HS_ISSW_BINARY}
					fi
					if [ -f ${B}/${config}/${UBOOT_HS_2ND_BINARY} ]; then
						install ${B}/${config}/${UBOOT_HS_2ND_BINARY} ${UBOOT_HS_2ND_IMAGE}-${type}
						rm -f ${UBOOT_HS_2ND_BINARY} ${UBOOT_HS_2ND_SYMLINK}
						ln -sf ${UBOOT_HS_2ND_IMAGE}-${type} ${UBOOT_HS_2ND_SYMLINK}-${type}
						ln -sf ${UBOOT_HS_2ND_IMAGE}-${type} ${UBOOT_HS_2ND_SYMLINK}
						ln -sf ${UBOOT_HS_2ND_IMAGE}-${type} ${UBOOT_HS_2ND_BINARY}-${type}
						ln -sf ${UBOOT_HS_2ND_IMAGE}-${type} ${UBOOT_HS_2ND_BINARY}
					fi
				fi
			done
			unset  j
		done
		unset  i
	else
		cd ${DEPLOYDIR}
		if [ "x${SPL_UART_BINARY}" != "x" ]; then
			install ${B}/spl/${SPL_UART_BINARY} ${SPL_UART_IMAGE}
			rm -f ${SPL_UART_BINARY} ${SPL_UART_SYMLINK}
			ln -sf ${SPL_UART_IMAGE} ${SPL_UART_BINARY}
			ln -sf ${SPL_UART_IMAGE} ${SPL_UART_SYMLINK}
		fi
		if [ -f ${B}/${UBOOT_HS_XLD_BINARY} ]; then
			install ${B}/${UBOOT_HS_XLD_BINARY} ${UBOOT_HS_XLD_IMAGE}
			rm -f ${UBOOT_HS_XLD_BINARY} ${UBOOT_HS_XLD_SYMLINK}
			ln -sf ${UBOOT_HS_XLD_IMAGE} ${UBOOT_HS_XLD_SYMLINK}
			ln -sf ${UBOOT_HS_XLD_IMAGE} ${UBOOT_HS_XLD_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_MLO_BINARY} ]; then
			install ${B}/${UBOOT_HS_MLO_BINARY} ${UBOOT_HS_MLO_IMAGE}
			rm -f ${UBOOT_HS_MLO_BINARY} ${UBOOT_HS_MLO_SYMLINK}
			ln -sf ${UBOOT_HS_MLO_IMAGE} ${UBOOT_HS_MLO_SYMLINK}
			ln -sf ${UBOOT_HS_MLO_IMAGE} ${UBOOT_HS_MLO_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_ISSW_BINARY} ]; then
			install ${B}/${UBOOT_HS_ISSW_BINARY} ${UBOOT_HS_ISSW_IMAGE}
			rm -f ${UBOOT_HS_ISSW_BINARY} ${UBOOT_HS_ISSW_SYMLINK}
			ln -sf ${UBOOT_HS_ISSW_IMAGE} ${UBOOT_HS_ISSW_SYMLINK}
			ln -sf ${UBOOT_HS_ISSW_IMAGE} ${UBOOT_HS_ISSW_BINARY}
		fi
		if [ -f ${B}/${UBOOT_HS_2ND_BINARY} ]; then
			install ${B}/${UBOOT_HS_2ND_BINARY} ${UBOOT_HS_2ND_IMAGE}
			rm -f ${UBOOT_HS_2ND_BINARY} ${UBOOT_HS_2ND_SYMLINK}
			ln -sf ${UBOOT_HS_2ND_IMAGE} ${UBOOT_HS_2ND_SYMLINK}
			ln -sf ${UBOOT_HS_2ND_IMAGE} ${UBOOT_HS_2ND_BINARY}
		fi
	fi

	rm ${DEPLOYDIR}/u-boot-initial-env || true
	rm ${DEPLOYDIR}/${PN}-initial-env || true
	rm ${DEPLOYDIR}/u-boot-initial-env-${MACHINE} || true
	rm ${DEPLOYDIR}/${PN}-initial-env-${MACHINE} || true
	rm ${DEPLOYDIR}/u-boot-initial-env-${MACHINE}-${PV}-${PR} || true
	rm ${DEPLOYDIR}/${PN}-initial-env-${MACHINE}-${PV}-${PR} || true
}

TOOLCHAIN = "gcc"
