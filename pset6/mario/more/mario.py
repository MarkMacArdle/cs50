from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
        break

# Print out the mario style half pyramids in hashs
for i in range(height):
    leading_whitespace = ' ' * (height - (i + 1))
    hashs = '#' * (i + 1)
    print(leading_whitespace + hashs + '  ' + hashs)