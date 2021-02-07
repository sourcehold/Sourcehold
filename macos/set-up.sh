#!/bin/sh

SAVES_DIR="saves"

echo "Setting up environment..."
if [ -d "$SAVES_DIR" ]; then
     echo "Creating 'saves' directory..."
     mkdir "$SAVES_DIR"
fi
