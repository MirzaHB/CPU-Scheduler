# John Zheng, 2024
# CPSC 457 F24 Teaching Assistant
# Students may use this code in Assignment 1 for graph generation

# what my output looks like, yours may not be exactly the same
"""
.................
50,     78,     100,    400,    900,    128,    822,    40 
Average wait time: 5.06 ms
Average turnaround time: 7.10 ms
Average response time: 3.00 ms
"""

# this Python script runs
# ./a.exe RR (number)
# for numbers 1-100
# It extracts the terminal output
# and writes the relevant numbers into a .csv
# Everything to be in the current working directory

import subprocess

rr_avgs = []
for i in range(1, 101):
    res = subprocess.run(
        ["./scheduler", "RR", str(i)],  # Corrected command
        stdin=open("Assignment 1 input file.csv"),
        capture_output=True,
    )
    # print(res.stdout.decode())  # This is your output
    averages = res.stdout.decode().split("\n")[-4:-1]  # Get the last three lines
    for j in range(3):  # Use a different variable to avoid overwriting 'i'
        averages[j] = float(
            averages[j].split()[-2]
        )  # Get the second last word and convert it to a float
    # print(averages)
    rr_avgs.append(averages)

with open("rr_avg.csv", "w") as f:
    f.write("Quantum,Wait,Turnaround,Response\n")
    for i in range(1, 101):
        f.write(f"{i},{rr_avgs[i-1][0]},{rr_avgs[i-1][1]},{rr_avgs[i-1][2]}\n")