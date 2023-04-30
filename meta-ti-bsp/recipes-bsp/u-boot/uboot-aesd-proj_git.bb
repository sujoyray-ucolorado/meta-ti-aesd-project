require u-boot-ti.inc

LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

PR = "r0"

PV = "2020.04"

# For the un-initiated:
# The actual URL you'd use with a git clone for example would be:
# https://source.denx.de/u-boot/u-boot.git/
# However, in the context of OE, we have to explicitly split things up:
# a) we want it to use git fetcher - hence git:// prefix in GIT_URI (if we
#  used https here, we'd endup attempting wget instead of git)
# b) and we want git fetcher to use https protocol, hence GIT_PROTOCOL as https


UBOOT_GIT_URI = "git://git@github.com/sujoyray-ucolorado/final-project-assignment-uboot-sujoyray.git"
UBOOT_GIT_PROTOCOL = "https"
SRCREV = "6a00f48beb303e7ef4fb92359e6ef9741ade6aa9"
BRANCH = "main"

