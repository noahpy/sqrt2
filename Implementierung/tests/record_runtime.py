
import subprocess
import time
import matplotlib.pyplot as plt
import sys

plt.ion()  # Enable interactive mode
plt.rcParams.update({'font.size': 17})

fig, ax = plt.subplots()

TEST_RANGE = 20000
STEP = 500
ITERATION = 1
VERSION_START = 0
VERSION_END = 3

times = [[] for _ in range(VERSION_END-VERSION_START+1)]  # Store the count values
precisions = []  # Store the i values

file_name = "runtime_graph.png"
if len(sys.argv) >= 2:
    file_name = sys.argv[1]

for i in range(1, TEST_RANGE + 1, STEP):
    precisions.append(i)
    ax.clear()

    for j in range(VERSION_START, VERSION_END + 1):
        counter = ITERATION
        runtime = 0
        while(counter):
            start = time.perf_counter()
            result = subprocess.run(['../main', f'-d{i}', f'-V{j}'], capture_output=True, text=True)
            end = time.perf_counter()
            runtime += (end - start)
            counter -= 1
        runtime /= ITERATION
        errors = result.stderr
               
        if errors:
            print(f"Received error messages when running ../main -d{i}:", errors)
            break

        times[j-1].append(runtime)


        # Plot the count vs. iterations graph
        ax.plot(precisions, times[j-1], label=f'Average runtime for Version {j} in correlation with demanded precision')
        ax.grid(True)
    ax.set_xlabel('precision in decimal digits')
    ax.set_ylabel(f'average runtime in s')

    # ax.legend()
    plt.draw()
    plt.pause(0.001)
    print(f"Done {(i / (TEST_RANGE-STEP) * 100):.2f}%", end="\r", flush=True)

    # Save the final graph to a file
    plt.savefig(file_name)




# Turn off interactive mode
plt.ioff()

# Display the graph
plt.show()





