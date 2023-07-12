import subprocess
import time
import matplotlib.pyplot as plt
import sys

plt.ion()  # Enable interactive mode

fig, ax = plt.subplots()

TEST_RANGE = 100000
STEP = 1000
ITERATION = 10

times = []  # Store the count values
precisions = []  # Store the i values



for i in range(1, TEST_RANGE + 1, STEP):
    counter = ITERATION
    runtime = 0
    while(counter):
        start = time.perf_counter()
        result = subprocess.run(['./runtime_performance', f'{ITERATION, i}'], capture_output=True, text=True)
        end = time.perf_counter()
        runtime += (end - start)
        counter -= 1
    runtime /= ITERATION
    errors = result.stderr
    
    if errors:
        print(f"Received error messages when running ../main -d{i}:", errors)
        break

    times.append(runtime)
    precisions.append(i)

    ax.clear()

    # Plot the count vs. iterations graph
    ax.plot(precisions, times, label='Average runtime in correlation with demanded precision')
    ax.set_xlabel('precision in decimal digits')
    ax.set_ylabel(f'average runtime of {ITERATION} iterations in s')
    ax.grid(True)

    ax.legend()
    plt.draw()
    plt.pause(0.001)
    print(f"Done {(i / (TEST_RANGE-STEP) * 100):.2f}%", end="\r", flush=True)

file_name = "runtime_graph.png"
if len(sys.argv) >= 2:
    file_name = sys.argv[1]
# Save the final graph to a file
plt.savefig(file_name)

# Turn off interactive mode
plt.ioff()

# Display the graph
plt.show()
