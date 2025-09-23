#!/bin/bash
#
# Apollo Build Script V3.5
# For Exynos9810
# Forked from Exynos8890 Script
# Coded by AnanJaser1211 @ 2019-2022
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Main Dir
CR_DIR=$(pwd)
# Compiler Dir
CR_TC=../compiler
# Target ARCH
CR_ARCH=arm64
# Define proper arch and dir for dts files
CR_DTS=arch/$CR_ARCH/boot/dts/exynos
# Define boot.img out dir
CR_OUT=$CR_DIR/Apollo/Out
CR_PRODUCT=$CR_DIR/Apollo/Product
# Kernel Zip Package
CR_ZIP=$CR_DIR/Apollo/kernelzip
CR_OUTZIP=$CR_OUT/kernelzip
# Presistant A.I.K Location
CR_AIK=$CR_DIR/Apollo/A.I.K
# Main Ramdisk Location
CR_RAMDISK=$CR_DIR/Apollo/Ramdisk
# Compiled image name and location (Image/zImage)
CR_KERNEL=$CR_DIR/arch/$CR_ARCH/boot/Image
# Compiled dtb by dtbtool
CR_DTB=$CR_DIR/arch/$CR_ARCH/boot/dtb.img
# defconfig dir
CR_DEFCONFIG=$CR_DIR/arch/$CR_ARCH/configs
# Kernel Name and Version
CR_VERSION=V1.11
CR_NAME=DS-ACK
# Thread count
CR_JOBS=$(nproc --all)
# Target Android version
CR_ANDROID=q
CR_PLATFORM=13.0.0
# Current Date
CR_DATE=$(date +%d.%m.%Y)
# General init
export ANDROID_MAJOR_VERSION=$CR_ANDROID
export PLATFORM_VERSION=$CR_PLATFORM
export $CR_ARCH
##########################################
# Device specific Variables [SM-G960X]
CR_CONFIG_G960=starlte_defconfig
CR_VARIANT_G960F=G960F
CR_VARIANT_G960N=G960N
# Device specific Variables [SM-G965X]
CR_CONFIG_G965=star2lte_defconfig
CR_VARIANT_G965F=G965F
CR_VARIANT_G965N=G965N
# Device specific Variables [SM-N960X]
CR_CONFIG_N960=crownlte_defconfig
CR_VARIANT_N960F=N960F
CR_VARIANT_N960N=N960N
# Common configs
CR_CONFIG_9810=exynos9810_defconfig
CR_CONFIG_SPLIT=NULL
CR_CONFIG_APOLLO=apollo_defconfig
CR_CONFIG_INTL=eur_defconfig
CR_CONFIG_KOR=kor_defconfig
CR_SELINUX="2"
CR_KSU="n"
CR_KSU_NEXT="n"
CR_SUSFS="n"
CR_CLEAN="n"
# Default Compilation
DEFAULT_TARGET=7   # Build All (All Devices ZIP)
DEFAULT_COMPILER=4 # clang20 (Google Clang 20.0.0 - Best for One UI 7)
DEFAULT_SELINUX=2  # enforce
DEFAULT_KSU=n      # disabled
DEFAULT_KSU_NEXT=y # enabled (recommended)
DEFAULT_SUSFS=y    # enabled (recommended for root concealment)
DEFAULT_CLEAN=n    # dirty
#####################################################

# KernelSU, KernelSU-Next and SuSFS Setup Functions
SETUP_KSU()
{
    echo "----------------------------------------------"
    echo " Setting up KernelSU (Classic)"
    
    # Run KernelSU setup script
    echo " Running KernelSU setup script..."
    curl -LSs "https://raw.githubusercontent.com/tiann/KernelSU/main/kernel/setup.sh" | bash -
    
    if [ $? -ne 0 ]; then
        echo " Failed to setup KernelSU"
        exit 1
    fi
    
    echo " KernelSU setup completed"
}

SETUP_KSU_NEXT()
{
    echo "----------------------------------------------"
    echo " Setting up KernelSU-Next (sidex15)"
    
    # Clean existing KernelSU-Next if exists
    if [ -d "KernelSU-Next" ]; then
        rm -rf KernelSU-Next
    fi
    
    # Clone KernelSU-Next from sidex15 with next-susfs branch
    echo " Cloning KernelSU-Next from sidex15..."
    git clone https://github.com/sidex15/KernelSU-Next.git --branch next-susfs --single-branch
    
    if [ $? -ne 0 ]; then
        echo " Failed to clone KernelSU-Next from sidex15"
        exit 1
    fi
    
    # Create symbolic link to drivers/kernelsu
    echo " Creating KernelSU symbolic link..."
    if [ -L "drivers/kernelsu" ]; then
        rm -f drivers/kernelsu
    fi
    if [ -d "drivers/kernelsu" ]; then
        rm -rf drivers/kernelsu
    fi
    
    ln -sf "../KernelSU-Next/kernel" "drivers/kernelsu"
    
    if [ $? -ne 0 ]; then
        echo " Failed to create KernelSU symbolic link"
        exit 1
    fi
    
    # Add KernelSU to drivers Makefile and Kconfig if not present
    if ! grep -q "kernelsu" drivers/Makefile; then
        echo "obj-\$(CONFIG_KSU) += kernelsu/" >> drivers/Makefile
        echo " Added KernelSU to drivers/Makefile"
    fi
    
    if ! grep -q "drivers/kernelsu/Kconfig" drivers/Kconfig; then
        sed -i '/endmenu/i\source "drivers/kernelsu/Kconfig"' drivers/Kconfig
        echo " Added KernelSU to drivers/Kconfig"
    fi
    
    # If SuSFS is being used, apply SuSFS patches to KernelSU-Next
    if [[ "$CR_SUSFS" =~ ^[yY]$ ]] && [ -d "susfs4ksu" ] && [ -f "susfs4ksu/kernel_patches/KernelSU/10_enable_susfs_for_ksu.patch" ]; then
        echo " Applying SuSFS patches to KernelSU-Next..."
        cd KernelSU-Next
        patch -p1 < ../susfs4ksu/kernel_patches/KernelSU/10_enable_susfs_for_ksu.patch
        if [ $? -eq 0 ]; then
            echo " SuSFS patches applied to KernelSU-Next successfully"
        else
            echo " Warning: Failed to apply SuSFS patches to KernelSU-Next"
            echo " This may be normal if KernelSU-Next already has SuSFS support built-in"
        fi
        cd ..
    fi
    
    echo " KernelSU-Next (sidex15) setup completed"
}

SETUP_SUSFS()
{
    echo "----------------------------------------------"
    echo " Setting up SuSFS (simonpunk) v1.5.9 with manual kernel patches"
    
    # Clean existing susfs4ksu if exists
    if [ -d "susfs4ksu" ]; then
        rm -rf susfs4ksu
    fi
    
    # Clean existing SuSFS files
    rm -f fs/susfs.c fs/sus_su.c include/linux/susfs.h include/linux/susfs_def.h include/linux/sus_su.h
    
    # Clone SuSFS from simonpunk with gki-android15-6.6 branch for v1.5.9
    echo " Cloning SuSFS v1.5.9 from simonpunk (gki-android15-6.6 branch)..."
    git clone https://gitlab.com/simonpunk/susfs4ksu.git --branch gki-android15-6.6 --single-branch
    
    if [ $? -ne 0 ]; then
        echo " Failed to clone SuSFS from simonpunk"
        exit 1
    fi
    
    echo " SuSFS v1.5.9 cloned successfully"
    
    # Copy SuSFS source files to kernel source
    echo " Integrating SuSFS v1.5.9 into kernel source..."
    
    # Copy fs files from kernel_patches/fs/ to fs/
    if [ -f "susfs4ksu/kernel_patches/fs/susfs.c" ]; then
        echo " Copying SuSFS fs files..."
        cp susfs4ksu/kernel_patches/fs/susfs.c fs/ 2>/dev/null || echo " Failed to copy susfs.c"
        cp susfs4ksu/kernel_patches/fs/sus_su.c fs/ 2>/dev/null || echo " Failed to copy sus_su.c"
    else
        echo " No SuSFS fs files found"
    fi
    
    # Copy include files from kernel_patches/include/linux/ to include/linux/
    if [ -f "susfs4ksu/kernel_patches/include/linux/susfs.h" ]; then
        echo " Copying SuSFS include files..."
        cp susfs4ksu/kernel_patches/include/linux/susfs.h include/linux/ 2>/dev/null || echo " Failed to copy susfs.h"
        cp susfs4ksu/kernel_patches/include/linux/susfs_def.h include/linux/ 2>/dev/null || echo " Failed to copy susfs_def.h"
        cp susfs4ksu/kernel_patches/include/linux/sus_su.h include/linux/ 2>/dev/null || echo " Failed to copy sus_su.h"
    else
        echo " No SuSFS include files found"
    fi
    
    # Apply manual kernel patches for SuSFS v1.5.9 integration
    echo " Applying manual kernel patches for SuSFS v1.5.9..."
    
    # 1. Update fs/Makefile to include SuSFS
    if ! grep -q "CONFIG_KSU_SUSFS" fs/Makefile; then
        echo " Adding SuSFS to fs/Makefile..."
        # Add after line containing obj-y assignments and before CONFIG_BUFFER_HEAD
        if grep -q "kernel_read_file.o.*remap_range.o" fs/Makefile; then
            sed -i '/kernel_read_file\.o.*remap_range\.o/a\\nobj-$(CONFIG_KSU_SUSFS) += susfs.o' fs/Makefile
        else
            # Fallback approach for kernel 4.9 structure
            sed -i '/^obj-y.*:=.*$/a\\nobj-$(CONFIG_KSU_SUSFS) += susfs.o' fs/Makefile
        fi
        echo " Added SuSFS to fs/Makefile"
    fi
    
    # 2. Add SuSFS include to fs/exec.c for SUS_SU support
    if [ -f "fs/exec.c" ] && ! grep -q "susfs_def.h" fs/exec.c; then
        echo " Adding SuSFS include to fs/exec.c..."
        # Add after asm/uaccess.h include (kernel 4.9 structure)
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS_SUS_SU\n#include <linux/susfs_def.h>\n#endif' fs/exec.c
        echo " Added SuSFS include to fs/exec.c"
    fi
    
    # 3. Add SuSFS include to fs/devpts/inode.c  
    if [ -f "fs/devpts/inode.c" ] && ! grep -q "susfs_def.h" fs/devpts/inode.c; then
        echo " Adding SuSFS include to fs/devpts/inode.c..."
        # Add after linux/seq_file.h include
        sed -i '/#include <linux\/seq_file.h>/a\#ifdef CONFIG_KSU_SUSFS_SUS_SU\n#include <linux/susfs_def.h>\n#endif' fs/devpts/inode.c
        echo " Added SuSFS include to fs/devpts/inode.c"
    fi
    
    # 4. Add SuSFS include to fs/namei.c
    if [ -f "fs/namei.c" ] && ! grep -q "susfs.h" fs/namei.c; then
        echo " Adding SuSFS include to fs/namei.c..."
        # Add after asm/uaccess.h include (consistent with kernel 4.9)
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS\n#include <linux/susfs.h>\n#endif' fs/namei.c
        echo " Added SuSFS include to fs/namei.c"
    fi
    
    # 5. Add SuSFS include to fs/namespace.c
    if [ -f "fs/namespace.c" ] && ! grep -q "susfs.h" fs/namespace.c; then
        echo " Adding SuSFS include to fs/namespace.c..."
        # Add after asm/uaccess.h include
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS\n#include <linux/susfs.h>\n#endif' fs/namespace.c
        echo " Added SuSFS include to fs/namespace.c"
    fi
    
    # 6. Add SuSFS include to fs/open.c
    if [ -f "fs/open.c" ] && ! grep -q "susfs.h" fs/open.c; then
        echo " Adding SuSFS include to fs/open.c..."
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS\n#include <linux/susfs.h>\n#endif' fs/open.c
        echo " Added SuSFS include to fs/open.c"
    fi
    
    # 7. Add SuSFS include to fs/readdir.c
    if [ -f "fs/readdir.c" ] && ! grep -q "susfs.h" fs/readdir.c; then
        echo " Adding SuSFS include to fs/readdir.c..."
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS\n#include <linux/susfs.h>\n#endif' fs/readdir.c
        echo " Added SuSFS include to fs/readdir.c"
    fi
    
    # 8. Add SuSFS include to fs/stat.c  
    if [ -f "fs/stat.c" ] && ! grep -q "susfs.h" fs/stat.c; then
        echo " Adding SuSFS include to fs/stat.c..."
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS\n#include <linux/susfs.h>\n#endif' fs/stat.c
        echo " Added SuSFS include to fs/stat.c"
    fi
    
    # 9. Add SuSFS include to fs/statfs.c
    if [ -f "fs/statfs.c" ] && ! grep -q "susfs.h" fs/statfs.c; then
        echo " Adding SuSFS include to fs/statfs.c..."
        sed -i '/#include <asm\/uaccess.h>/a\#ifdef CONFIG_KSU_SUSFS\n#include <linux/susfs.h>\n#endif' fs/statfs.c
        echo " Added SuSFS include to fs/statfs.c"
    fi
    
    # Note about compilation
    echo " Manual SuSFS v1.5.9 kernel patches applied"
    echo " Note: Additional runtime hooks may need manual integration during compilation"
    echo " If compilation fails, check for missing function calls in the modified files"
    
    echo " SuSFS v1.5.9 setup completed with manual kernel integration"
}

CLEAN_KSU_SUSFS()
{
    echo " Cleaning KernelSU/SuSFS setup..."
    
    # Remove KernelSU symlink and directories
    if [ -L "drivers/kernelsu" ]; then
        rm -f drivers/kernelsu
        echo " Removed KernelSU symlink"
    fi
    if [ -d "drivers/kernelsu" ]; then
        rm -rf drivers/kernelsu
        echo " Removed KernelSU directory"
    fi
    
    # Remove KernelSU repositories
    if [ -d "KernelSU" ]; then
        rm -rf KernelSU
        echo " Removed KernelSU directory"
    fi
    if [ -d "KernelSU-Next" ]; then
        rm -rf KernelSU-Next
        echo " Removed KernelSU-Next directory"
    fi
    
    # Remove SuSFS
    if [ -d "susfs4ksu" ]; then
        rm -rf susfs4ksu
        echo " Removed susfs4ksu directory"
    fi
    if [ -d "fs/susfs" ]; then
        rm -rf fs/susfs
        echo " Removed SuSFS kernel module"
    fi
    
    # Clean KernelSU from drivers/Makefile
    if [ -f "drivers/Makefile" ]; then
        sed -i '/kernelsu/d' drivers/Makefile
        echo " Cleaned KernelSU from drivers/Makefile"
    fi
    
    # Clean KernelSU from drivers/Kconfig
    if [ -f "drivers/Kconfig" ]; then
        sed -i '/drivers\/kernelsu\/Kconfig/d' drivers/Kconfig
        echo " Cleaned KernelSU from drivers/Kconfig"
    fi
    
    echo " KernelSU/SuSFS cleanup completed"
}

# Compiler Selection
BUILD_COMPILER()
{

# Auto download and setup compilers
# For manually adding compiler, add it under
# Apollo/toolchain/clang-custom and select option 7

# Clang Versions and features

if [ $CR_COMPILER = "1" ]; then
CR_CLANG_URL=https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/heads/llvm-r416183/clang-r416183.tar.gz
CR_CLANG=$CR_TC/clang-12.0.4-r416183
fi
if [ $CR_COMPILER = "2" ]; then
CR_CLANG_URL=https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/heads/llvm-r450784/clang-r450784b.tar.gz
CR_CLANG=$CR_TC/clang-14.0.4-r450784
fi
if [ $CR_COMPILER = "3" ]; then
CR_CLANG_URL=https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/heads/llvm-r522817/clang-r522817.tar.gz
CR_CLANG=$CR_TC/clang-18.0.1-r522817
fi
if [ $CR_COMPILER = "4" ]; then
CR_CLANG_URL=https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/heads/main/clang-r547379.tar.gz
CR_CLANG=$CR_TC/clang-20.0.0-r547379
fi
if [ $CR_COMPILER = "5" ]; then
CR_CLANG_URL=https://github.com/Neutron-Toolchains/clang-build-catalogue/releases/download/05012024/neutron-clang-05012024.tar.zst
CR_CLANG=$CR_TC/neutron-clang-18.0.0
fi
if [ $CR_COMPILER = "6" ]; then
CR_CLANG_URL=https://github.com/Neutron-Toolchains/clang-build-catalogue/releases/download/10032024/neutron-clang-10032024.tar.zst
CR_CLANG=$CR_TC/neutron-clang-19.0.0
fi
if [ $CR_COMPILER = "7" ]; then
CR_CLANG_URL=https://github.com/Neutron-Toolchains/clang-build-catalogue/releases/download/15052024/neutron-clang-15052024.tar.zst
CR_CLANG=$CR_TC/neutron-clang-20.0.0
fi
if [ $CR_COMPILER = "8" ]; then
CR_CLANG=$CR_TC/clang-custom
fi

if [ $CR_COMPILER != "8" ]; then
    if [ ! -d "$CR_CLANG/bin" ] || [ ! -d "$CR_CLANG/lib" ]; then
        echo " "
        echo " $CR_CLANG compiler is missing"
        echo " "
        echo " Automatically downloading toolchain..."
        
        URL=$CR_CLANG_URL
        if curl --output /dev/null --silent --head --fail "$URL"; then
            echo "URL exists: $URL"
            echo "Downloading $CR_CLANG"
            if [ ! -e $CR_TC ]; then
                mkdir $CR_TC
            fi
            if [ ! -e $CR_CLANG ]; then
                mkdir $CR_CLANG
            else
                # Remove incomplete
                rm -rf $CR_CLANG
                mkdir $CR_CLANG
            fi
            
            # Determine extraction method based on file extension
            if [[ "$URL" == *.tar.zst ]]; then
                # For .tar.zst files (Neutron toolchains)
                wget -qO- $URL | tar --use-compress-program=unzstd -xv -C $CR_CLANG
            else
                # For .tar.gz files (Google toolchains)
                wget -qO- $URL | tar -xzv -C $CR_CLANG
            fi
            
            if [ $? -ne 0 ]; then
                echo "Download failed or was incomplete"
                echo "Retrying with alternative method..."
                rm -rf $CR_CLANG
                mkdir $CR_CLANG
                
                # Alternative download method
                wget -O toolchain_temp.tar "$URL"
                if [[ "$URL" == *.tar.zst ]]; then
                    tar --use-compress-program=unzstd -xvf toolchain_temp.tar -C $CR_CLANG
                else
                    tar -xzf toolchain_temp.tar -C $CR_CLANG
                fi
                rm -f toolchain_temp.tar
                
                if [ $? -ne 0 ]; then
                    echo "Download failed completely"
                    echo "Setup Compiler and try again"
                    exit 0;
                fi
            fi
            
            # Neutron Needs patches
            if [ $CR_COMPILER = "5" ] || [ $CR_COMPILER = "6" ] || [ $CR_COMPILER = "7" ]; then
                cd $CR_CLANG
                bash <(curl -s "https://raw.githubusercontent.com/Neutron-Toolchains/antman/main/antman") --patch=glibc
                cd $CR_DIR
            fi
            echo "Compiler Downloaded automatically."
        else
            echo "Invalid URL: $URL"
            echo "Please check your internet connection and try again"
            exit 0;
        fi
    fi
else
    if [ ! -d "$CR_CLANG/bin" ] || [ ! -d "$CR_CLANG/lib" ]; then
        echo "clang-custom compiler is missing in $CR_TC/clang-custom"
        exit 0;
    fi
fi

# Clang Features (18 and higher)
if [ $CR_COMPILER -ge 3 ]; then
export CONFIG_THINLTO=y
export CONFIG_UNIFIEDLTO=y
export CONFIG_LLVM_MLGO_REGISTER=y
export CONFIG_LLVM_POLLY=y
export CONFIG_LLVM_DFA_JUMP_THREAD=y
fi

# Additional optimizations for Clang 20 (One UI 7 optimized)
if [ $CR_COMPILER = "4" ]; then
echo "Enabling Clang 20 One UI 7 optimizations..."
export CONFIG_LTO_CLANG_FULL=y
export CONFIG_CFI_CLANG=y
export CONFIG_SHADOW_CALL_STACK=y
fi

export PATH=$CR_CLANG/bin:$CR_CLANG/lib:${PATH}
export CC=$CR_CLANG/bin/clang
export REAL_CC=$CR_CLANG/bin/clang
export LD=$CR_CLANG/bin/ld.lld
export AR=$CR_CLANG/bin/llvm-ar
export NM=$CR_CLANG/bin/llvm-nm
export OBJCOPY=$CR_CLANG/bin/llvm-objcopy
export OBJDUMP=$CR_CLANG/bin/llvm-objdump
export READELF=$CR_CLANG/bin/llvm-readelf
export STRIP=$CR_CLANG/bin/llvm-strip
export LLVM=1
export KALLSYMS_EXTRA_PASS=1
export ARCH=arm64 && export SUBARCH=arm64
compile="make ARCH=arm64 CC=clang"
CR_COMPILER_ARG="$CR_CLANG"
}

# Clean-up Function

BUILD_CLEAN()
{
if [[ "$CR_CLEAN" =~ ^[yY]$ ]]; then
     $compile clean && $compile mrproper
     rm -r -f $CR_DTB
     rm -r -f $CR_KERNEL
     rm -rf $CR_DTS/.*.tmp
     rm -rf $CR_DTS/.*.cmd
     rm -rf $CR_DTS/*.dtb
     rm -rf $CR_DIR/.config
     rm -rf $CR_OUT/*.img
     rm -rf $CR_OUT/*.zip
     CLEAN_KSU_SUSFS
else
     rm -r -f $CR_DTB
     rm -r -f $CR_KERNEL
     rm -rf $CR_DTS/.*.tmp
     rm -rf $CR_DTS/.*.cmd
     rm -rf $CR_DTS/*.dtb
     rm -rf $CR_DIR/.config
     rm -rf $CR_DIR/.version
fi
}

# Kernel Name Function

BUILD_IMAGE_NAME()
{
    CR_IMAGE_NAME=$CR_NAME-$CR_VERSION-$CR_VARIANT-$CR_DATE
    zver=$CR_NAME-$CR_VERSION-$CR_DATE
    
}

# Build options
BUILD_OPTIONS()
{
    # KSU Version Detection
    KSU_VERSION=""
    if [[ "$CR_KSU" =~ ^[yY]$ ]]; then
        KSU_VERSION=$( [ -f "drivers/kernelsu/Makefile" ] && grep -oP '(?<=-DKSU_VERSION=)[0-9]+' drivers/kernelsu/Makefile )
    elif [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]]; then
        KSU_VERSION=$( [ -f "drivers/kernelsu/Makefile" ] && grep -oP '(?<=-DKSU_VERSION=)[0-9]+' drivers/kernelsu/Makefile )
    fi
    
    echo "----------------------------------------------"
    echo " Apollo Kernel Build Options "
    echo " "
    echo " Kernel		- $CR_IMAGE_NAME"
    echo " Device		- $CR_VARIANT"
    echo " Compiler	- $CR_COMPILER_ARG"
    if [[ "$CR_CLEAN" =~ ^[yY]$ ]]; then
        echo " Env		- Clean Build"
    else
        echo " Env		- Dirty Build"
    fi
    if [ $CR_SELINUX = "1" ]; then
        echo " SELinux	- Permissive"
    else
        echo " SELinux	- Enforcing"
    fi
    if [[ "$CR_KSU" =~ ^[yY]$ ]]; then
        if [ -n "$KSU_VERSION" ]; then
        echo " KernelSU	- Version: $KSU_VERSION"
        else
        echo " KernelSU	- Enabled"
        fi
    elif [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]]; then
        if [ -n "$KSU_VERSION" ]; then
        echo " KernelSU-Next	- Version: $KSU_VERSION"
        else
        echo " KernelSU-Next	- Enabled"
        fi
    else
        echo " KernelSU	- Disabled"
    fi
    if [[ "$CR_SUSFS" =~ ^[yY]$ ]]; then
        echo " SuSFS		- Enabled"
    else
        echo " SuSFS		- Disabled"
    fi
    echo " "
}

# Config Generation Function

BUILD_GENERATE_CONFIG()
{
  # Only use for devices that are unified with 2 or more configs
  echo "----------------------------------------------"
  echo " Generating defconfig for $CR_VARIANT"
  echo " "
  # Respect CLEAN build rules
  BUILD_CLEAN
  if [ -e $CR_DEFCONFIG/tmp_defconfig ]; then
    echo " Clean-up old config "
    rm -rf $CR_DEFCONFIG/tmp_defconfig
  fi
  echo " Base	- $CR_CONFIG "
  cp -f $CR_DEFCONFIG/$CR_CONFIG $CR_DEFCONFIG/tmp_defconfig
  # Split-config support for devices with unified defconfigs (Universal + device)
  if [ $CR_CONFIG_SPLIT = NULL ]; then
    echo " No split config support! "
  else
    echo " Device - $CR_CONFIG_SPLIT "
    cat $CR_DEFCONFIG/$CR_CONFIG_SPLIT >> $CR_DEFCONFIG/tmp_defconfig
  fi
  # Regional Config
  echo " Region	- $CR_CONFIG_REGION "
  cat $CR_DEFCONFIG/$CR_CONFIG_REGION >> $CR_DEFCONFIG/tmp_defconfig
  # Apollo Custom defconfig
  echo " Apollo	- $CR_CONFIG_APOLLO "
  cat $CR_DEFCONFIG/$CR_CONFIG_APOLLO >> $CR_DEFCONFIG/tmp_defconfig
  # Selinux Never Enforce all targets
  if [ $CR_SELINUX = "1" ]; then
    echo " Building SELinux Permissive Kernel"
    echo "CONFIG_ALWAYS_PERMISSIVE=y" >> $CR_DEFCONFIG/tmp_defconfig
    CR_IMAGE_NAME=$CR_IMAGE_NAME-Permissive
    zver=$zver-Permissive
  else
    echo " Building SELinux Enforced Kernel"
  fi
  
  # KernelSU Configuration
  if [[ "$CR_KSU" =~ ^[yY]$ ]]; then
    echo " Building with KernelSU (Classic)"
    SETUP_KSU
    echo "CONFIG_KSU=y" >> $CR_DEFCONFIG/tmp_defconfig
    CR_IMAGE_NAME=$CR_IMAGE_NAME-KSU
    zver=$zver-KernelSU
  elif [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]]; then
    echo " Building with KernelSU-Next"
    SETUP_KSU_NEXT
    echo "CONFIG_KSU=y" >> $CR_DEFCONFIG/tmp_defconfig
    CR_IMAGE_NAME=$CR_IMAGE_NAME-KSU-Next
    zver=$zver-KernelSU-Next
  else
    echo " Building without root (creating dummy KernelSU Kconfig)"
    # Create a dummy KernelSU Kconfig file to prevent build errors
    mkdir -p drivers/kernelsu
    echo "# Dummy KernelSU Kconfig - no KernelSU support" > drivers/kernelsu/Kconfig
    echo "# CONFIG_KSU is not set" >> $CR_DEFCONFIG/tmp_defconfig
  fi
  
  # SuSFS Configuration
  if [[ "$CR_SUSFS" =~ ^[yY]$ ]]; then
    echo " Building with SuSFS"
    SETUP_SUSFS
    echo "CONFIG_SUSFS=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_FMASK=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_OPEN_REDIRECT=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_SUS_MOUNT=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_SUS_KSTAT=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_SUS_OVERLAYFS=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_TRY_UMOUNT=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_SPOOF_UNAME=y" >> $CR_DEFCONFIG/tmp_defconfig
    echo "CONFIG_SUSFS_SUS_SU=y" >> $CR_DEFCONFIG/tmp_defconfig
    CR_IMAGE_NAME=$CR_IMAGE_NAME-SuSFS
    zver=$zver-SuSFS
  else
    echo "# CONFIG_SUSFS is not set" >> $CR_DEFCONFIG/tmp_defconfig
  fi
  echo " $CR_VARIANT config generated "
  echo " "
  CR_CONFIG=tmp_defconfig
}

# Kernel information Function
BUILD_OUT()
{
# KSU Version
    KSU_VERSION=""
    if [[ "$CR_KSU" =~ ^[yY]$ ]] || [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]]; then
        KSU_VERSION=$( [ -f "drivers/kernelsu/Makefile" ] && grep -oP '(?<=-DKSU_VERSION=)[0-9]+' drivers/kernelsu/Makefile )
    fi
  echo "----------------------------------------------"
  echo " Kernel		- $CR_IMAGE_NAME"
  echo " Device		- $CR_VARIANT"
  echo " Compiler	- $CR_COMPILER_ARG"
    if [[ "$CR_CLEAN" =~ ^[yY]$ ]]; then
        echo " Env		- Clean Build"
    else
        echo " Env		- Dirty Build"
    fi
    if [ $CR_SELINUX = "1" ]; then
        echo " SELinux	- Permissive"
    else
        echo " SELinux	- Enforcing"
    fi
  if [[ "$CR_KSU" =~ ^[yY]$ ]]; then
    echo " KernelSU	- Version: $KSU_VERSION"
  elif [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]]; then
    echo " KernelSU-Next	- Version: $KSU_VERSION"
  fi
  if [[ "$CR_SUSFS" =~ ^[yY]$ ]]; then
    echo " SuSFS		- Enabled"
  fi
  echo "----------------------------------------------"
  echo "$CR_VARIANT kernel build finished."
  echo "Compiled DTB Size = $sizdT Kb"
  echo "Kernel Image Size = $sizT Kb"
  echo "Boot Image   Size = $sizkT Kb"
  echo "$CR_PRODUCT/$CR_IMAGE_NAME.img Ready"
  echo "Press Any key to end the script"
  echo "----------------------------------------------"
}

# Kernel Compile Function
BUILD_ZIMAGE()
{
	echo "----------------------------------------------"
	echo " "
	echo "Building zImage for $CR_VARIANT"
	export LOCALVERSION=-$CR_IMAGE_NAME
	echo "Make $CR_CONFIG"
	$compile $CR_CONFIG
	echo "Make Kernel with $CR_COMPILER_ARG"
	$compile -j$CR_JOBS
	if [ ! -e $CR_KERNEL ]; then
	exit 0;
	echo "Image Failed to Compile"
	echo " Abort "
	fi
	du -k "$CR_KERNEL" | cut -f1 >sizT
	sizT=$(head -n 1 sizT)
	rm -rf sizT
	echo " "
	echo "----------------------------------------------"
}

# Device-Tree compile Function
BUILD_DTB()
{
	echo "----------------------------------------------"
	echo " "
	echo "Checking DTB for $CR_VARIANT"
	# This source does compiles dtbs while doing Image
	if [ ! -e $CR_DTB ]; then
        exit 0;
        echo "DTB Failed to Compile"
        echo " Abort "
	else
        echo "DTB Compiled at $CR_DTB"
	fi
	rm -rf $CR_DTS/.*.tmp
	rm -rf $CR_DTS/.*.cmd
	rm -rf $CR_DTS/*.dtb
	du -k "$CR_DTB" | cut -f1 >sizdT
	sizdT=$(head -n 1 sizdT)
	rm -rf sizdT
	echo " "
	echo "----------------------------------------------"
}

# Ramdisk Function
PACK_BOOT_IMG()
{
	echo "----------------------------------------------"
	echo " "
	echo "Building Boot.img for $CR_VARIANT"
	# Copy Ramdisk
	cp -rf $CR_RAMDISK/* $CR_AIK
	# Move Compiled kernel and dtb to A.I.K Folder
	mv $CR_KERNEL $CR_AIK/split_img/boot.img-zImage
	mv $CR_DTB $CR_AIK/split_img/boot.img-dtb
	# Create boot.img
	$CR_AIK/repackimg.sh
	if [ ! -e $CR_AIK/image-new.img ]; then
        exit 0;
        echo "Boot Image Failed to pack"
        echo " Abort "
	fi
	# Remove red warning at boot
	echo -n "SEANDROIDENFORCE" >> $CR_AIK/image-new.img
	# Copy boot.img to Production folder
	if [ ! -e $CR_PRODUCT ]; then
        mkdir $CR_PRODUCT
	fi
	cp $CR_AIK/image-new.img $CR_PRODUCT/$CR_IMAGE_NAME.img
	# Move boot.img to out dir
	if [ ! -e $CR_OUT ]; then
        mkdir $CR_OUT
	fi
	mv $CR_AIK/image-new.img $CR_OUT/$CR_IMAGE_NAME.img
	du -k "$CR_OUT/$CR_IMAGE_NAME.img" | cut -f1 >sizkT
	sizkT=$(head -n 1 sizkT)
	rm -rf sizkT
	echo " "
	$CR_AIK/cleanup.sh
	# Respect CLEAN build rules
	BUILD_CLEAN
}

# Single Target Build Function
BUILD()
{
	if [ "$CR_TARGET" = "1" ]; then
		echo " Galaxy S9 INTL"
		CR_CONFIG_SPLIT=$CR_CONFIG_G960
		CR_CONFIG_REGION=$CR_CONFIG_INTL
		CR_VARIANT=$CR_VARIANT_G960F
		export "CONFIG_MACH_EXYNOS9810_STARLTE_EUR_OPEN=y"
	fi
	if [ "$CR_TARGET" = "2" ]; then
		echo " Galaxy S9+ INTL"
		CR_CONFIG_SPLIT=$CR_CONFIG_G965
		CR_CONFIG_REGION=$CR_CONFIG_INTL
		CR_VARIANT=$CR_VARIANT_G965F
		export "CONFIG_MACH_EXYNOS9810_STAR2LTE_EUR_OPEN=y"
	fi
	if [ "$CR_TARGET" = "3" ]
	then
		echo " Galaxy Note9 INTL"
		CR_CONFIG_SPLIT=$CR_CONFIG_N960
		CR_CONFIG_REGION=$CR_CONFIG_INTL
		CR_VARIANT=$CR_VARIANT_N960F
		export "CONFIG_MACH_EXYNOS9810_CROWNLTE_EUR_OPEN=y"
	fi
	if [ "$CR_TARGET" = "4" ]; then
		echo " Galaxy S9 KOR"
		CR_CONFIG_SPLIT=$CR_CONFIG_G960
		CR_CONFIG_REGION=$CR_CONFIG_KOR
		CR_VARIANT=$CR_VARIANT_G960N
		export "CONFIG_MACH_EXYNOS9810_STARLTE_KOR=y"
	fi
	if [ "$CR_TARGET" = "5" ]; then
		echo " Galaxy S9+ KOR"
		CR_CONFIG_SPLIT=$CR_CONFIG_G965
		CR_CONFIG_REGION=$CR_CONFIG_KOR
		CR_VARIANT=$CR_VARIANT_G965N
		export "CONFIG_MACH_EXYNOS9810_STAR2LTE_KOR=y"
	fi
	if [ "$CR_TARGET" = "6" ]
	then
		echo " Galaxy Note9 KOR"
		CR_CONFIG_SPLIT=$CR_CONFIG_N960
		CR_CONFIG_REGION=$CR_CONFIG_KOR
		CR_VARIANT=$CR_VARIANT_N960N
		export "CONFIG_MACH_EXYNOS9810_CROWNLTE_KOR=y"
	fi	
	CR_CONFIG=$CR_CONFIG_9810
	BUILD_COMPILER
	BUILD_CLEAN
	BUILD_IMAGE_NAME
	BUILD_GENERATE_CONFIG
	# Print build options
	BUILD_OPTIONS
	BUILD_ZIMAGE
	BUILD_DTB
	if [ "$CR_MKZIP" = "y" ]; then # Allow Zip Package for mass compile only
	echo " Start Build ZIP Process "
	PACK_KERNEL_ZIP
	else
	PACK_BOOT_IMG
	BUILD_OUT
	fi
}

# Multi-Target Build Function
BUILD_ALL(){
echo "----------------------------------------------"
echo " Compiling ALL targets "
CR_TARGET=1
BUILD
export -n "CONFIG_MACH_EXYNOS9810_STARLTE_EUR_OPEN"
CR_TARGET=2
BUILD
export -n "CONFIG_MACH_EXYNOS9810_STAR2LTE_EUR_OPEN"
CR_TARGET=3
BUILD
export -n "CONFIG_MACH_EXYNOS9810_CROWNLTE_EUR_OPEN"
CR_TARGET=4
BUILD
export -n "CONFIG_MACH_EXYNOS9810_STARLTE_KOR"
CR_TARGET=5
BUILD
export -n "CONFIG_MACH_EXYNOS9810_STAR2LTE_KOR"
CR_TARGET=6
BUILD
export -n "CONFIG_MACH_EXYNOS9810_CROWNLTE_KOR"
}

# Preconfigured Debug build
BUILD_DEBUG(){
echo "----------------------------------------------"
echo " DEBUG : Debug build initiated "
CR_TARGET=5
CR_COMPILER=3
CR_SELINUX=0
CR_KSU="n"
CR_KSU_NEXT="y"
CR_SUSFS="y"
CR_CLEAN="n"
echo " DEBUG : Set Build options "
echo " DEBUG : Variant  : $CR_VARIANT_N960F"
echo " DEBUG : Compiler : Clang 18"
echo " DEBUG : Selinux  : $CR_SELINUX Enforcing"
echo " DEBUG : KSU-Next : $CR_KSU_NEXT"
echo " DEBUG : SuSFS    : $CR_SUSFS"
echo " DEBUG : Clean    : $CR_CLEAN"
echo "----------------------------------------------"
BUILD
echo "----------------------------------------------"
echo " DEBUG : build completed "
echo "----------------------------------------------"
exit 0;
}


# Pack All Images into ZIP
PACK_KERNEL_ZIP() {
echo "----------------------------------------------"
echo " Packing ZIP "

# Variables
CR_BASE_KERNEL=$CR_OUTZIP/floyd/G960F-kernel
CR_BASE_DTB=$CR_OUTZIP/floyd/G960F-dtb

# Check packages
if ! dpkg-query -W -f='${Status}' bsdiff  | grep "ok installed" >/dev/null 2>&1; then 
	echo "bsdiff is missing and is required for ZIP Packaging."
	
	# Auto-install for CI/Automated builds
	if [ "$CI" = "true" ] || [ "$AUTOMATED_BUILD" = "true" ] || [ -n "$GITHUB_ACTIONS" ]; then
		echo "Automated build detected. Installing bsdiff automatically..."
		sudo apt update -qq
		sudo apt install -y -qq bsdiff
		if ! dpkg-query -W -f='${Status}' bsdiff | grep "ok installed" >/dev/null 2>&1; then
			echo "Failed to install bsdiff automatically."
			exit 1
		fi
		echo "bsdiff installed successfully."
	else
		read -p "Do you want to install bsdiff? This requires sudo privileges. (y/n) > " INSTALL_BSDIFF
		if [ "$INSTALL_BSDIFF" = "y" ]; then
			echo "installing bsdiff."
			sudo apt update
			sudo apt install -y bsdiff
			if ! dpkg-query -W -f='${Status}' bsdiff | grep "ok installed" >/dev/null 2>&1; then
				echo "Failed to install bsdiff. Please try installing it manually."
				exit 0;
			fi
		else
			echo "Please install bsdiff with sudo apt install bsdiff and try again."
			exit 0;
		fi
	fi
fi

# Initalize with base image (Starlte)
if [ "$CR_TARGET" = "1" ]; then # Always must run ONCE during BUILD_ALL otherwise fail. Setup directories
	echo " "
	echo " Kernel Zip Packager "
	echo " Base Target "
	echo " Clean Out directory "
	echo " "
	rm -rf $CR_OUTZIP
	cp -r $CR_ZIP $CR_OUTZIP
	echo " "
	echo " Copying $CR_BASE_KERNEL "
	echo " Copying $CR_BASE_DTB "
	echo " "
	if [ ! -e $CR_KERNEL ] || [ ! -e $CR_DTB ]; then
        exit 0;
        echo " Kernel not found!"
        echo " Abort "
	else
        cp $CR_KERNEL $CR_BASE_KERNEL
        cp $CR_DTB $CR_BASE_DTB
	fi
	# Set kernel version
fi
if [ ! "$CR_TARGET" = "1" ]; then # Generate patch files for non starlte kernels
	echo " "
	echo " Kernel Zip Packager "
	echo " "
	echo " Generating Patch kernel for $CR_VARIANT "
	echo " "
	if [ ! -e $CR_KERNEL ] || [ ! -e $CR_DTB ]; then
        echo " Kernel not found! "
        echo " Abort "
        exit 0;
	else
		bsdiff $CR_BASE_KERNEL $CR_KERNEL $CR_OUTZIP/floyd/$CR_VARIANT-kernel
		if [ ! -e $CR_OUTZIP/floyd/$CR_VARIANT-kernel ]; then
			echo "ERROR: bsdiff $CR_BASE_KERNEL $CR_KERNEL $CR_OUTZIP/floyd/$CR_VARIANT-kernel Failed!"
			exit 0;
		fi
		bsdiff $CR_BASE_DTB $CR_DTB $CR_OUTZIP/floyd/$CR_VARIANT-dtb
		if [ ! -e $CR_OUTZIP/floyd/$CR_VARIANT-kernel ]; then
			echo "ERROR: bsdiff $CR_BASE_KERNEL $CR_DTB $CR_OUTZIP/floyd/$CR_VARIANT-dtb Failed!"
			exit 0;
		fi
	fi
fi
if [ "$CR_TARGET" = "6" ]; then # Final kernel build
	echo " Generating ZIP Package for $CR_NAME-$CR_VERSION-$CR_DATE"
	sed -i "s/fkv/$zver/g" $CR_OUTZIP/META-INF/com/google/android/update-binary
	cd $CR_OUTZIP && zip -r $CR_PRODUCT/$zver.zip * && cd $CR_DIR
	du -k "$CR_PRODUCT/$zver.zip" | cut -f1 >sizdz
	sizdz=$(head -n 1 sizdz)
	rm -rf sizdz
	echo " "
	echo "----------------------------------------------"
	echo "$CR_NAME kernel build finished."
	echo "Compiled Package Size = $sizdz Kb"
	echo "$zver.zip Ready"
	echo "Press Any key to end the script"
	echo "----------------------------------------------"
fi
}

# Automated Build Function for CI/CD
BUILD_AUTOMATED()
{
    # Use environment variables or default values
    CR_TARGET=${CR_TARGET:-$DEFAULT_TARGET}
    CR_COMPILER=${CR_COMPILER:-4}  # Default to Clang 20 for One UI 7
    CR_SELINUX=${CR_SELINUX:-$DEFAULT_SELINUX}
    
    # Handle KSU flags from environment variables
    if [ -n "$CR_KSU" ]; then
        CR_KSU=$CR_KSU
    else
        CR_KSU=$DEFAULT_KSU
    fi
    
    if [ -n "$CR_KSU_NEXT" ]; then
        CR_KSU_NEXT=$CR_KSU_NEXT
    else
        CR_KSU_NEXT=$DEFAULT_KSU_NEXT
    fi
    
    if [ -n "$CR_SUSFS" ]; then
        CR_SUSFS=$CR_SUSFS
    else
        CR_SUSFS=$DEFAULT_SUSFS
    fi
    
    CR_CLEAN=${CR_CLEAN:-$DEFAULT_CLEAN}
    
    echo "----------------------------------------------"
    echo " AUTOMATED BUILD MODE (One UI 7 Optimized)"
    echo " Target: $CR_TARGET"
    echo " Compiler: Google Clang 20.0.0 (Option $CR_COMPILER)"
    echo " SELinux: $CR_SELINUX"
    echo " KSU: $CR_KSU"
    echo " KSU-Next: $CR_KSU_NEXT"
    echo " SuSFS: $CR_SUSFS"
    echo " Clean: $CR_CLEAN"
    echo "----------------------------------------------"
    
    # Set root choice based on individual flags
    if [[ "$CR_KSU" =~ ^[yY]$ ]]; then
        CR_ROOT_CHOICE=2
    elif [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]] && [[ "$CR_SUSFS" =~ ^[yY]$ ]]; then
        CR_ROOT_CHOICE=4
    elif [[ "$CR_KSU_NEXT" =~ ^[yY]$ ]]; then
        CR_ROOT_CHOICE=3
    else
        CR_ROOT_CHOICE=1
    fi
    
    # Execute build
    if [ "$CR_TARGET" = "7" ]; then
        CR_MKZIP="y"
        BUILD_ALL
    else
        BUILD
    fi
}

# Check if running in automated mode (CI environment)
if [ "$CI" = "true" ] || [ "$AUTOMATED_BUILD" = "true" ] || [ -n "$GITHUB_ACTIONS" ]; then
    echo "Running in automated mode..."
    BUILD_AUTOMATED
    exit 0
fi

# Main Menu
clear
echo "----------------------------------------------"
echo "$CR_NAME $CR_VERSION Build Script $CR_DATE"
echo "Optimized for One UI 7 with Google Clang 20.0.0"
echo "Default: All Devices ZIP with KernelSU-Next + SuSFS"
if [ "$1" = "-d" ]; then
BUILD_DEBUG
fi
echo " "
echo " "
echo "1) starlte" "   2) star2lte" "   3) crownlte"
echo "4) starltekor" "5) star2ltekor" "6) crownltekor"
echo  " "
echo "7) Build All/ZIP (RECOMMENDED)"  "8) Abort"
echo "----------------------------------------------"
read -p "Please select your build target (1-8) [Default: 7 - All Devices] > " CR_TARGET
echo "----------------------------------------------"
echo " "
echo "1) Google Clang 12 (LLVM +LTO)"
echo "2) Google Clang 14 (LLVM +LTO)"
echo "3) Google Clang 18 (LLVM +LTO PGO Bolt MLGO Polly)"
echo "4) Google Clang 20 (LLVM +LTO PGO Bolt MLGO Polly) [RECOMMENDED for One UI 7]"
echo "5) Neutron Clang 18 (Advanced optimizations)"
echo "6) Neutron Clang 19 (Advanced optimizations)"
echo "7) Neutron Clang 20 (Latest, experimental)"
echo "8) Other (Apollo/toolchain/clang-custom)"
echo " "
read -p "Please select your compiler (1-8) [Default: 4] > " CR_COMPILER
echo " "
echo "1) SELinux Permissive "  "2) SELinux Enforcing"
echo " "
read -p "Please select your SElinux mode (1-2) [Default: 2 - Enforcing] > " CR_SELINUX
echo " "
echo "Root Solutions:"
echo "1) None"
echo "2) KernelSU (Classic)"
echo "3) KernelSU-Next"
echo "4) KernelSU-Next + SuSFS (RECOMMENDED for banking apps)"
echo " "
read -p "Please select root solution (1-4) [Default: 4 - KSU-Next+SuSFS] > " CR_ROOT_CHOICE
echo " "

# Set root options based on choice
case $CR_ROOT_CHOICE in
    1)
        CR_KSU="n"
        CR_KSU_NEXT="n"
        CR_SUSFS="n"
        ;;
    2)
        CR_KSU="y"
        CR_KSU_NEXT="n"
        CR_SUSFS="n"
        ;;
    3)
        CR_KSU="n"
        CR_KSU_NEXT="y"
        CR_SUSFS="n"
        ;;
    4)
        CR_KSU="n"
        CR_KSU_NEXT="y"
        CR_SUSFS="y"
        ;;
    *)
        CR_KSU="n"
        CR_KSU_NEXT="n"
        CR_SUSFS="n"
        ;;
esac

if [ "$CR_TARGET" = "8" ]; then
echo "Build Aborted"
exit
fi
echo " "
read -p "Clean Builds? (y/n) > " CR_CLEAN
echo " "

# Validate options
if ! [[ "$CR_TARGET" =~ ^[1-8]$ ]]; then
    CR_TARGET=$DEFAULT_TARGET
    echo " No target selected, defaulting to Build All/ZIP (All Devices)"
fi

if ! [[ "$CR_COMPILER" =~ ^[1-8]$ ]]; then
    CR_COMPILER=$DEFAULT_COMPILER
    echo " No compiler selected, defaulting to Google Clang 20.0.0 (One UI 7 optimized)"
fi

if ! [[ "$CR_SELINUX" =~ ^[1-2]$ ]]; then
    CR_SELINUX=$DEFAULT_SELINUX
    echo " No SELinux mode selected, defaulting to Enforcing"
fi

if ! [[ "$CR_ROOT_CHOICE" =~ ^[1-4]$ ]]; then
    CR_KSU=$DEFAULT_KSU
    CR_KSU_NEXT=$DEFAULT_KSU_NEXT
    CR_SUSFS=$DEFAULT_SUSFS
    echo " No root solution selected, defaulting to KernelSU-Next + SuSFS"
fi

if ! [[ "$CR_CLEAN" =~ ^[yYnN]$ ]]; then
    CR_CLEAN=$DEFAULT_CLEAN
    echo " No clean option selected, defaulting to dirty build"
fi

# Call functions
if [ "$CR_TARGET" = "7" ]; then
echo " "
read -p "Build Flashable ZIP ? (y/n) [Default: y] > " CR_MKZIP
if ! [[ "$CR_MKZIP" =~ ^[yYnN]$ ]]; then
    CR_MKZIP="y"
    echo " Defaulting to flashable ZIP creation"
fi
echo " "
BUILD_ALL
else
BUILD
fi