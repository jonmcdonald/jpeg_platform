#######################################################################
#
# LINUX OS BUILD PROCESS - README
# In this README.txt, we assume using of tcsh as a user shell
# For ARM (A9MP) Toolchain and Linux 2.6.35.12 Distribution 
#
#######################################################################
cd Building_Linux
# Set current directory as build directory $BUILD_DIR
setenv BUILD_DIR `pwd`

# Choose/create "downloads" directory and download the open source stuff 
setenv DOWNLOAD_DIR `pwd`/downloads

# Install the toolchain into $TOOL_CHAIN directory; verify that 
# $TOOL_CHAIN/bin/arm-none-linux-gnueabi-gcc exist
tar jxf $DOWNLOAD_DIR/arm-2010.09-50-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
setenv TOOL_CHAIN `pwd`/arm-2010.09

# Extract "Linux_Build_resources.tgz" under the $BUILD_DIR
# ($BUILD_DIR/Linux_Build_resources should present)
cd $BUILD_DIR
tar -xzf resources/Linux_Build_resources.tgz

# Verify that "cpio" command takes "--owner=root:root -o" argument;
# use latest versions (2.10 is fine)
cpio --owner=root:root -o
# In case if "cpio: --owner is meaningless with --create" error is given,
# build the cpio 2.10:
tar -jxf $DOWNLOAD_DIR/cpio-2.10.tar.bz2
cd cpio-2.10
./configure
make -j 8
setenv PATH `pwd`/src:$PATH
cd $BUILD_DIR
cpio --version
# Now "cpio --version" should show "2.10"

# Open the sources into $BUILD_DIR:
tar -jxf $DOWNLOAD_DIR/linux-2.6.35.12.tar.bz2
tar -jxf $DOWNLOAD_DIR/busybox-1.17.4.tar.bz2

# Set the Kernel build environment
setenv TARGET arm-none-linux-gnueabi
setenv CROSS_COMPILE $TOOL_CHAIN/bin/arm-none-linux-gnueabi-
setenv ARCH arm

# Copy initial sysroot (text files only)
cd $BUILD_DIR
cp -a Linux_Build_resources/sysroot $BUILD_DIR
setenv SYSROOT $BUILD_DIR/sysroot

# Git cloning - instead of patches
# git clone /home/shure/git-repo/linux-2.6.35.y.git

# Apply Kernel patch and build kernel
cd $BUILD_DIR
cd linux-2.6.35.12
patch -p1 < ../patches/linux-2.6.35.patch
make mgc_a9mp_defconfig
make menuconfig

# In the open GUI do the following:
#  -select: System Type
#  -select: MGC DEMO Platform Settings
#  -goto line: Enable UART1 support
#  -press shift+n which should disable UART1, result in:
#   [] Enable UART1 support 
#  -goto line: Physical memory size
#  -change size from 0x10000000 to 0x0F000000
#  -exit at the main menu
#  -select: file systems
#  -goto line: []FUSE (filesystem in user space) support
#  -press shift+y which should result in
#   [*] FUSE (filesystem in user space) support
#  -exit at the bottom (twice)

make -j 8
make headers_install INSTALL_HDR_PATH=$SYSROOT/usr
# Later, you can use $BUILD_DIR/linux-2.6.35.12/vmlinux file
# in order to debug linux kernel.

# Initial sysroot
cd $BUILD_DIR
cp resources/mk-sysroot .
./mk-sysroot

# Build the h264 guc driver module
cd linux-2.6.35.12
make M=$BUILD_DIR/Linux_Build_resources/guc_h264_driver modules
make M=$BUILD_DIR/Linux_Build_resources/guc_h264_driver INSTALL_MOD_PATH=$SYSROOT modules_install
cd $BUILD_DIR

# Build Jpeg Utils (djpeg)
cd $BUILD_DIR/Linux_Build_resources/jpeg_utils/jpeg-6b/
./configure CC=${CROSS_COMPILE}gcc
cp -a djpeg $SYSROOT/root
cd $BUILD_DIR

# Build the jpeg driver module
cd linux-2.6.35.12
make M=$BUILD_DIR/Linux_Build_resources/jpeg_driver modules
make M=$BUILD_DIR/Linux_Build_resources/jpeg_driver INSTALL_MOD_PATH=$SYSROOT modules_install 
cd $BUILD_DIR

# Copy glibc from the toolchain
cp -a $TOOL_CHAIN/arm-none-linux-gnueabi/libc/etc $SYSROOT
cp -a $TOOL_CHAIN/arm-none-linux-gnueabi/libc/lib $SYSROOT
cp -a $TOOL_CHAIN/arm-none-linux-gnueabi/libc/sbin $SYSROOT
cp -a $TOOL_CHAIN/arm-none-linux-gnueabi/libc/usr $SYSROOT

# Build Busybox
cd busybox-1.17.4
make defconfig
make -j 8
make install CONFIG_PREFIX=$SYSROOT
cd $BUILD_DIR

# Build TestApp
cd sysroot/root/testapp
make CC=${CROSS_COMPILE}gcc
cd $BUILD_DIR

# Build LCD support apps
cd sysroot/root/lcd_support
${CROSS_COMPILE}gcc -g fbinit.c -o $SYSROOT/root/fbinit
${CROSS_COMPILE}gcc -g lcdfill.c -o $SYSROOT/root/lcdfill
cd $BUILD_DIR

# Build h264 user application
cd sysroot/root/h264/
${CROSS_COMPILE}gcc -g h264_user.c  -o h264_user
${CROSS_COMPILE}gcc -g h264_user_4p.c  -o $SYSROOT/root/h264_user_4p.x
cp -a $SYSROOT/root/h264/guc_load $SYSROOT/root
cd $BUILD_DIR

# Build jpeg user applications
cd Linux_Build_resources/sysroot/root/jpeg_programs
${CROSS_COMPILE}gcc -I ../../../jpeg_driver -g -o $SYSROOT/root/jpeg_program jpeg_program.c bmp.c
${CROSS_COMPILE}gcc -g -o $SYSROOT/root/bmpshow  bmpshow.c bmp.c
cp -p $BUILD_DIR/Linux_Build_resources/jpeg_driver/jpeg_load $SYSROOT/root

cd $BUILD_DIR
cp resources/ShanaTova800x600.bmp $SYSROOT/root
cp resources/fractal_800x600.bmp $SYSROOT/root
cp resources/mobile_q20_60.264 $SYSROOT/root
cp resources/*.sh $SYSROOT/root

# Build the initrd and bootp
Linux_Build_resources/dump_sysroot 
make -C Linux_Build_resources/bootp BUILD_DIR=$BUILD_DIR MACH_ID=3388

# Now, you can run the Linux_Build_resources/bootp/bootp image
# NOTE: The file "bootp" is your Linux image that needs to be specified in Vista
# parameter file e.g.: 
# top.CPU_Cortex_A9.PV.cpu0.core.elf_image_file = swlnx/bootp

cd <demo-install-dir>/sw_linux_bootp
mv bootp bootp.old
ln -s $BUILD_DIR/Linux_Build_resources/bootp/bootp .
