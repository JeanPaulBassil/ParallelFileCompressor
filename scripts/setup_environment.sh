#!/bin/bash

# Check if the directory exists
if [ ! -d "$1" ]; then
    echo "Directory does not exist"
    exit 1
fi

# Initialize a counter
counter=1

# Find all files in the directory (excluding directories)
find "$1" -maxdepth 1 -type f | while read file; do
    # Get the file extension
    extension="${file##*.}"
    # Rename the file to the current counter value with its original extension
    mv "$file" "$1/$counter.$extension"
    # Increment the counter
    ((counter++))
done

exit 0
