import random
import math

TESTCASE1 = "TC_RandomPoints"
TESTCASE2 = "TC_LinePoints"
TESTCASE3 = "TC_CirclePoints"
TESTCASE4 = "TC_SquarePoints"
TESTCASE5 = "TC_LargeDataset"

TESTCASE_VARIANTS = [10, 50, 1000]

def generate_random_points(filename, num_points):
    with open(filename, 'w') as f:
        f.write(f"{num_points}\n")
        for _ in range(num_points):
            x = random.uniform(0, 1280)
            y = random.uniform(0, 1024)
            f.write(f"{x} {y}\n")

# Generate variants with 10, 50, and 1000 data points
for variant in TESTCASE_VARIANTS:
    generate_random_points(f'{TESTCASE1}_{variant}.txt', variant)

def generate_line_points(filename, num_points):
    with open(filename, 'w') as f:
        f.write(f"{num_points}\n")
        x_increment = 1280.0 / (num_points - 1)
        y_increment = 1024.0 / (num_points - 1)
        for i in range(num_points):
            x = x_increment * i
            y = y_increment * i
            f.write(f"{x} {y}\n")

# Generate variants with 10, 50, and 1000 data points
for variant in TESTCASE_VARIANTS:
    generate_line_points(f'{TESTCASE2}_{variant}.txt', variant)

def generate_circle_points(filename, num_points):
    with open(filename, 'w') as f:
        f.write(f"{num_points}\n")
        center_x = 640.0  # Center of the window in x
        center_y = 512.0  # Center of the window in y
        radius = min(1280, 1024) * 0.4  # Radius is 40% of the smaller dimension
        for i in range(num_points):
            angle = 2 * math.pi * i / num_points
            x = center_x + radius * math.cos(angle)
            y = center_y + radius * math.sin(angle)
            f.write(f"{x} {y}\n")

# Generate variants with 10, 50, and 1000 data points
for variant in TESTCASE_VARIANTS:
    generate_circle_points(f'{TESTCASE3}_{variant}.txt', variant)

def generate_square_points(filename, num_points):
    with open(filename, 'w') as f:
        f.write(f"{num_points}\n")
        min_x = 320.0  # Left boundary
        max_x = 960.0  # Right boundary
        min_y = 256.0  # Top boundary
        max_y = 768.0  # Bottom boundary
        for _ in range(num_points):
            x = random.uniform(min_x, max_x)
            y = random.uniform(min_y, max_y)
            f.write(f"{x} {y}\n")

# Generate variants with 10, 50, and 1000 data points
for variant in TESTCASE_VARIANTS:
    generate_square_points(f'{TESTCASE4}_{variant}.txt', variant)

def generate_large_dataset(filename, num_points):
    with open(filename, 'w') as f:
        f.write(f"{num_points}\n")
        for i in range(num_points):
            x = random.uniform(0, 1280)
            y = random.uniform(0, 1024)
            f.write(f"{x} {y}\n")
            if i % 1000000 == 0 and i != 0:
                print(f"Generated {i} points.")

# Usage:
# Be cautious with this function as it will generate a very large file (~2GB or more)
generate_large_dataset(f'{TESTCASE5}.txt', 100_000_000)

