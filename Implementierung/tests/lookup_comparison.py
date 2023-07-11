
import subprocess
import matplotlib.pyplot as plt
import numpy as np

with open("digit.txt", "r") as f:
    lookup = f.read()

TEST_RANGE = 1000
STEP = 5

counts = []  # Store the count values
iterations = []  # Store the i values

plt.ion()  # Enable interactive mode

fig, ax = plt.subplots()

for i in range(1, TEST_RANGE + 1, STEP):
    result = subprocess.run(['../main', f'-d{i}'], capture_output=True, text=True)
    output = result.stdout
    errors = result.stderr
    count = 0

    for digit in lookup:
        if count >= len(output):
            print("Reached the end of the output.")
            break
        if output[count] == digit:
            count += 1
        else:
            break

    print(f"Matched {count - 2} digits when requested for {i}. Realtive Error: {abs(i-(count-2))/i}")
    if  i > count - 2:
        print("Fever digits than expected!")
    if errors:
        print("Received error messages:", errors)

    counts.append(count - 2)
    iterations.append(i)

    ax.clear()

    # Plot the count vs. iterations graph
    ax.plot(iterations, counts, label='Demanded vs Given precision of Sqrt 2')
    ax.set_xlabel('Demanded Precision')
    ax.set_ylabel('Resulting Precision')
    ax.grid(True)

    # Plot the additional function y = 2x + 50
    x = np.arange(1, i + 1, STEP)
    y = x
    ax.plot(x, y, label='y = x')

    # Update the plot
    ax.legend()
    plt.draw()
    plt.pause(0.001)

# Save the final graph to a file
plt.savefig('precision_comparison.png')

# Turn off interactive mode
plt.ioff()

# Display the graph
plt.show()
