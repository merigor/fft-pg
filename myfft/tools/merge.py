#! /usr/bin/env python3


if __name__ == "__main__":
    import sys
    from pathlib import Path
    pathlist = Path(sys.argv[1]).glob('**/*.txt')
    res = []
    for path in pathlist:
        data = []
        with open(str(path), 'r') as f:
            data = f.readlines()
        res += data

    target = Path(sys.argv[1] + "/result.txt")
    with open(str(target), 'w') as f:
        f.write("".join(res))
