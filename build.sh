#!/bin/bash
# Build script for Box2D QML

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building Box2D QML${NC}"

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}CMake not found. Please install CMake.${NC}"
    exit 1
fi

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning existing build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo -e "${GREEN}Configuring...${NC}"
cmake .. "$@"

# Build
echo -e "${GREEN}Building...${NC}"
cmake --build . -j$(nproc 2>/dev/null || echo 4)

echo -e "${GREEN}Build complete!${NC}"
echo ""
echo "To install, run: sudo cmake --install ."
echo "To run examples: cd examples && ./example_boxes"
