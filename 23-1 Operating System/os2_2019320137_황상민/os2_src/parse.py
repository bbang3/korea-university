import re
import sys
p1 = sys.argv[1]
p2 = sys.argv[2]

p1_burst = 0
p2_burst = 0
with open("./log_dmesg.txt", "r") as f:
    lines = f.readlines()
    for line in lines:
        subtext = line.split(',')[1]
        burst_time = int(re.sub(r'[^0-9]', '', subtext))
        # print(burst_time)
        if p1 in line:
            p1_burst += burst_time
        elif p2 in line:
            p2_burst += burst_time
        else:
            print("No proccess matched")
            break

print(f"{p1}: {p1_burst}")
print(f"{p2}: {p2_burst}")
print(f"total: {p1_burst+p2_burst}")
