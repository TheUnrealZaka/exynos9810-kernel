#!/bin/bash

# Set environment for testing
export CI=true
export AUTOMATED_BUILD=true
export CR_TARGET=1  # Single device test
export CR_COMPILER=4  # Clang 20
export CR_SELINUX=2   # Enforcing
export CR_KSU=n
export CR_KSU_NEXT=y
export CR_SUSFS=y
export CR_CLEAN=n

# Test the setup functions only
echo "Testing KernelSU-Next and SuSFS setup..."

source apollo.sh
unset CI AUTOMATED_BUILD GITHUB_ACTIONS

CLEAN_KSU_SUSFS

SETUP_KSU_NEXT

SETUP_SUSFS

echo "Setup completed successfully!"

# Check if symlink exists
if [ -L "drivers/kernelsu" ]; then
    echo "✓ KernelSU symlink created successfully"
    ls -la drivers/kernelsu
else
    echo "✗ KernelSU symlink missing"
fi

# Check if SuSFS files exist
if [ -d "fs/susfs" ]; then
    echo "✓ SuSFS files integrated successfully"
    ls -la fs/susfs/ | head -5
else
    echo "✗ SuSFS files missing"
fi

# Check if Kconfig is updated
if grep -q "drivers/kernelsu/Kconfig" drivers/Kconfig; then
    echo "✓ KernelSU added to drivers/Kconfig"
else
    echo "✗ KernelSU missing from drivers/Kconfig"
fi

# Check if Makefile is updated
if grep -q "kernelsu" drivers/Makefile; then
    echo "✓ KernelSU added to drivers/Makefile"
else
    echo "✗ KernelSU missing from drivers/Makefile"
fi

echo "Test completed!"
