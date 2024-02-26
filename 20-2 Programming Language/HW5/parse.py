


f = open("typeof_testcases")
fw = open("typeof_tc", "w")
fw2 = open("typeof_sol", "w")

s = f.read()
lst = s.lstrip("{").rstrip("}").split("\n")
lst = lst[1:len(lst)-2]

parsed_tc = []
for i in lst:
    parsed_tc = i.strip().split("=>")
    fw.write(parsed_tc[0] + "\n")
    fw2.write(parsed_tc[1] + "\n")


f.close()