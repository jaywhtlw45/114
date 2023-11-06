import random

# Define the dimensions of the 2D array
rows = 5
columns = 3

# Create a 2D list to store the random numbers
matrix = [[(random.randint(1, 15), random.randint(1, 10)) for _ in range(columns)] for _ in range(rows)]

# Print the generated 2D array with the specified format
for row in matrix:
    print(" ".join(map(str, row)))
