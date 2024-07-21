#!/usr/bin/env python3

import os
import string

# Verify string has only ASCII char
def is_ascii(s):
    return all(c in string.printable for c in s)

# Find file and dir. with non-ASCII name
def find_non_ascii_files(directory):
    non_ascii_files = []
    
    for root, dirs, files in os.walk(directory):
        # Check dir.
        for d in dirs:
            if not is_ascii(d):
                non_ascii_files.append(os.path.join(root, d))
        # Check file
        for f in files:
            if not is_ascii(f):
                non_ascii_files.append(os.path.join(root, f))
    
    return non_ascii_files

def main():
    import sys
    
    if len(sys.argv) != 2:
        print("Usage: onlyascii <directory>")
        sys.exit(1)
    
    directory = sys.argv[1]
    
    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a valid directory")
        sys.exit(1)
    
    non_ascii_files = find_non_ascii_files(directory)
    
    if non_ascii_files:
        print("Files and directories with non-ASCII characters:")
        for path in non_ascii_files:
            print(path)
    else:
        print("All file and directory names are ASCII.")

if __name__ == "__main__":
    main()
