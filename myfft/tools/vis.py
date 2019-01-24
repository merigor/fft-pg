#! /usr/bin/env python3


if __name__ == "__main__":
    import sys
    import matplotlib.pyplot as mpl

    data = []
    with open(sys.argv[1], 'r') as f:
        data = f.readlines()
    data = [i.split("=") for i in data if i.strip() != ""]
    # print(data)
    data = [(int(i[0][1:-2]), float(i[1][1:-1])) for i in data]
    x = [i[0] for i in data]
    y = [i[1] for i in data]
    mpl.plot(x, y)
    mpl.show()
