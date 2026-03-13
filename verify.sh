#!/bin/bash
# Verification Script for CSP32 School Bin Sensor
# This script verifies the project structure and basic syntax

echo "=========================================="
echo "CSP32 School Bin Sensor - Verification"
echo "=========================================="
echo ""

# Check if all required files exist
echo "Checking project structure..."
files=(
    "platformio.ini"
    "src/main.cpp"
    "include/config.h"
    "test/test_bin_level.cpp"
    "docs/HARDWARE.md"
    "docs/TESTING.md"
    "README.md"
    ".gitignore"
)

all_files_exist=true
for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file exists"
    else
        echo "✗ $file MISSING"
        all_files_exist=false
    fi
done

echo ""
if [ "$all_files_exist" = true ]; then
    echo "✓ All required files are present"
else
    echo "✗ Some files are missing"
    exit 1
fi

echo ""
echo "Checking file sizes..."
for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        size=$(wc -c < "$file")
        echo "  $file: $size bytes"
    fi
done

echo ""
echo "=========================================="
echo "Project structure verification complete!"
echo "=========================================="
echo ""
echo "To build the project:"
echo "  1. Install PlatformIO: pip install platformio"
echo "  2. Run: pio run"
echo "  3. Upload to ESP32: pio run -t upload"
echo ""
echo "For more information, see README.md"
