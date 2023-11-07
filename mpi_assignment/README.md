# README

## usage

1. Open your terminal and navigate to the directory containing the Makefile.
2. To compile the program, use the command: `make`
3. To run the program across multiple nodes, use the command: `make run`
4. To clean up the compiled files after you're done, use the command: `make clean`


## using the python script to setup hosts

1. Open your terminal and navigate to the directory containing the `setup_remotes.py` script.
2. Ensure you have the `paramiko` library installed. If not, install it using pip: `pip install paramiko`
3. Run the script with the following command: `python setup_remotes.py <hostsfile> <your-username> <your-password>`

## using the other python script to check if the hosts have mpi
1. it will output the hosts that have it to a new file
2. `python check_mpicc host_file username password output_file`
3. this also depends on `paramiko`