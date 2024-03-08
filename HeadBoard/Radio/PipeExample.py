import os

# FIFO file path
pipe_path = "/tmp/radio_to_main_pipe"

# Create a FIFO pipe if it does not exist
if not os.path.exists(pipe_path):
    os.mkfifo(pipe_path)

print("Initializing Pipe Example...")

while True:
    # Take input from user
    user_input = input()

    # Open FIFO for write only
    with open(pipe_path, "w") as pipe:
        # Write the user input to FIFO
        pipe.write(user_input)

    # Open FIFO for read only
    with open(pipe_path, "r") as pipe:
        # Read the data from FIFO
        read_data = pipe.read()

    # Print the read message
    print(f'From Command Handler: {read_data}', end="")
