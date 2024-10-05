import subprocess

# Generate alpha values from 0.1 to 1.0 in increments of 0.1
alpha_values = [round(0.1 * i, 1) for i in range(1, 11)]

srt_avgs = []
for alpha in alpha_values:
    # Run the scheduler command with SRT and the current alpha value
    res = subprocess.run(
        ["./scheduler", "SRT", str(alpha)],
        stdin=open("Assignment 1 input file.csv"),
        capture_output=True,
        text=True  # Automatically decode output to string
    )
    # Check if the command executed successfully
    if res.returncode != 0:
        print(f"Command failed with return code {res.returncode} for alpha = {alpha}")
        print(f"Error output: {res.stderr}")
        continue  # Skip to the next alpha value

    # Extract all lines containing the averages
    output_lines = res.stdout.strip().split("\n")

    # Initialize variables
    wait_time = None
    turnaround_time = None
    response_time = None

    # Search through all output lines
    for line in output_lines:
        if 'Average waiting time:' in line:
            try:
                wait_time = float(line.strip().split()[-2])
            except ValueError:
                print(f"Failed to parse waiting time in line: '{line}' for alpha = {alpha}")
        elif 'Average turnaround time:' in line:
            try:
                turnaround_time = float(line.strip().split()[-2])
            except ValueError:
                print(f"Failed to parse turnaround time in line: '{line}' for alpha = {alpha}")
        elif 'Average response time:' in line:
            try:
                response_time = float(line.strip().split()[-2])
            except ValueError:
                print(f"Failed to parse response time in line: '{line}' for alpha = {alpha}")

    # Check if all values were found
    if wait_time is not None and turnaround_time is not None and response_time is not None:
        srt_avgs.append([alpha, wait_time, turnaround_time, response_time])
    else:
        print(f"Could not find all average times for alpha = {alpha}")
        print(f"Skipping alpha = {alpha} due to parsing error.")

# Write the results to a CSV file
with open("srt_avg.csv", "w") as f:
    f.write("Alpha,Wait,Turnaround,Response\n")
    for averages in srt_avgs:
        f.write(f"{averages[0]},{averages[1]},{averages[2]},{averages[3]}\n")