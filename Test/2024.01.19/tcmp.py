#!/usr/bin/env python3
import os
import sys

# return set with all dir and file path from root_dir
def get_all_paths(root_dir):
    all_paths = set()
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for dirname in dirnames:
            all_paths.add(os.path.relpath(os.path.join(dirpath, dirname), root_dir))
        for filename in filenames:
            all_paths.add(os.path.relpath(os.path.join(dirpath, filename), root_dir))
    return all_paths

def main():
    #check if correct usage
    if len(sys.argv) == 3:
        mode = "common"
        dir1 = sys.argv[1]
        dir2 = sys.argv[2]
    elif len(sys.argv) == 4:
        mode = sys.argv[1]
        dir1 = sys.argv[2]
        dir2 = sys.argv[3]
        if mode not in ["-1", "-2"]:
            print("Bad Usage. Correct: tcmp [-1|-2] dir1 dir2")
            sys.exit(1)
    else:
        print("Usage: tcmp [-1|-2] dir1 dir2")
        sys.exit(1)

    #get all paths from dir.
    paths_dir1 = get_all_paths(dir1)
    paths_dir2 = get_all_paths(dir2)

    if mode == "common":
        common_paths = paths_dir1 & paths_dir2
        print("Common in subtrees: \n")
        for path in sorted(common_paths):
            print(path)
    elif mode == "-1":
        unique_dir1 = paths_dir1 - paths_dir2
        print("Exist only in first subtree: \n")
        for path in sorted(unique_dir1):
            print(path)
    elif mode == "-2":
        unique_dir2 = paths_dir2 - paths_dir1
        print("Exist only in second subtree: \n")
        for path in sorted(unique_dir2):
            print(path)

if __name__ == "__main__":
    main()
    