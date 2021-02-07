#!/bin/sh

PROJECT_ROOT=".."
SAVES_DIR="$PROJECT_ROOT/saves"

echo "Setting up environment..."
if [ ! -d "$SAVES_DIR" ]; then
     echo "Creating 'saves' directory..."
     mkdir "$SAVES_DIR"
fi
