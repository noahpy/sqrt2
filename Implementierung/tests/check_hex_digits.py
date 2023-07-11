import subprocess


TEST_RANGE = 10000
STEP = 1

counts = []  # Store the count values
iterations = []  # Store the i values



for i in range(1, TEST_RANGE + 1, STEP):
    result = subprocess.run(['../main', f'-h{i}'], capture_output=True, text=True)
    output = result.stdout.strip()
    errors = result.stderr
    
    count = len(output)

    print(f"Matched {count - 4} digits when requested for {i}. Realtive Error: {abs(i-(count-4))/i}")
    if  i > count - 4:
        print("Fever digits than expected!")
    if errors:
        print("Received error messages:", errors)

   
