#! /usr/bin/env python3
# extracts fft test results into a csv


# extracts number of threads from the string
def omp_string(s):
    return ("threads", int(s[-2]))


# extracts compiler name, problem size, data type size and number of repetitions
def compiler_string(s):
    res = [("compiler", s[1][:-1])]
    res.append(("N", int(s[4])))
    res.append(("real_t", int(s[9])))
    res.append(("R", int(s[-1])))
    return res


# extracts initialization time
def init_string(s):
    return ("init", float(s[1]))


# extracts duration of one repetition
def rep_string(s):
    return ("time", float(s[-1]))


# extracts total runtime
def total_string(s):
    return ("total", float(s[-1]))


# takes a folder as input,
# iterates over all files with extension "txt"
# extracts all the information in a hashmap
# flattens hashmap to a list
# prints the semicolon-separated list
if __name__ == "__main__":
    import sys
    from pathlib import Path

    # for every textfile in a given folder
    pathlist = Path(sys.argv[1]).glob('*.txt')
    res = []
    for path in pathlist:
        data = []
        entry = {"file": str(path)}
        # read data
        with open(str(path), 'r') as f:
            data = f.readlines()
        for line in data:
            i = line.split()

            if i[0].startswith("compiler"):
                if "compiler" in entry:
                    res.append(entry)
                    entry = {"file": str(path)}
                tmp = compiler_string(i)
                for j in tmp:
                    entry[j[0]] = j[1]

            elif i[0].startswith("init"):
                tmp = init_string(i)
                entry[tmp[0]] = tmp[1]

            elif i[0].startswith("total"):
                tmp = total_string(i)
                entry[tmp[0]] = tmp[1]

            elif i[0].startswith("tid") or i[0].startswith("rep"):
                if "times" in entry:
                    entry["times"].append(rep_string(i)[1])
                else:
                    entry["times"] = [rep_string(i)[1]]

        res.append(entry)

    # prints semicolon-separated values
    for i in res:
        row = []
        for k in i:
            if type(i[k]) is list:
                row += i[k]
            else:
                row.append(i[k])
        for j in row:
            print(j, end="; ")
        print("")
