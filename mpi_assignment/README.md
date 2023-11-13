# README

# Tutorial

## Connecting to the machines

### Connect to your head node

1. SSH into a machine that has MPI Working.
   1. One such example is `gojiro.cs.nmsu.edu`
2. Now let's refer to this machine as your head node.
3. How to tell if the machine has mpi?
   1. Connect to the machine
   2. try to run the command `mpirun --version` or `mpicc --version`
   3. It should print out the version number. If not it probably doesn't
   4. However sometimes your `PATH` is broken.
      1. To check this run this command
         1. `/usr/lib64/mpi/gcc/openmpi4/bin/mpicc --version`
         2. If this command returns the version number of mpicc then your `PATH` is broken.
            1. You don't have to fix it. 
               1. From now on you can just use `/usr/lib64/mpi/gcc/openmpi4/bin/mpicc` instead of `mpicc`
            2. If you want to try fixing it you just need to add `/usr/lib64/mpi/gcc/openmpi4/bin/` to your `PATH`
               1. [How to do this ](https://www.howtogeek.com/658904/how-to-add-a-directory-to-your-path-in-linux/)

### Connect to and setup your worker nodes

#### Find worker nodes that have mpi working.
1. Find at least 1 other worker that has mpi working.
2. Here is a list of known nodes (append `.cs.nmsu.edu` if you need to)
   1. `kraken`
   2. `gojiro`
   3. `tsunami`
   4. `kaiju`
   5. `gray`
      1. (`gray` had issues when I used it but if you use it as your head node it should work.)

#### Setup your worker nodes for no-password ssh login
1. This is necessary for mpi to be able to login to each node and execute commands without passwords
2. Don't worry this only allows your head node to do this. It is completely safe.
3. Steps
   1. Generate a local ssh key
      1. `ssh-keygen`
         1. Press enter for default options
   2. Copy the ssh key to one of the nodes you want to use
      1. `ssh-copy-id yourusername@yournode.cs.nmsu.edu`
      2. You only need to copy it to one node because the CS machines share the same files so copying it to one node will allow passwordless login on all of them.
   5. Test the ssh connection to each node
      1. for each node in your hostfile run this command
         1. `ssh yourusername@nodename.cs.nmsu.edu`
      2. each one should work without a password.
      3. This step is crucial because ssh wants you to confirm you want each hostname in your list of known hosts.

#### Create your "hosts" file
1. Create a plain text file in your code directory
   1. `touch mynodes.txt`
      1. You can name it anything you like
2. Put the hostnames of each of your nodes including your head node line-by-line (example below)

##### Example hosts file
```
kraken
gojiro
tsunami
kaiju

```

## Compile and run your code

### Compile
1. compile your c program that uses mpi using `mpicc`
2. `mpicc -o hello hello.c`

### Run
1. Run your code using `mpirun` and determine the number of processor slots
2. `mpirun --hostfile remotes.txt -np 2 ./hello`
3. If you setup enough hosts at least 2 processors should work.
4. Slowly increase the number after `-np` until it stops working. 
   1. This will tell you the max number of processor slots your node cluster can provide. 
   2. In my case I was able to reach 100 slots with 5 machines

## Troubleshooting

### Duplicate hosts in hostfile
1. If you have duplicates in your hostfile or you included `localhost` as well as the public name of your head host your code will break

### The infamous newline issue

1. Sometimes having a newline at the bottom of your hosts file can break or fix things
2. Try with and without a blank line at the bottom of your hosts file

### My code is just stuck

1. Some nodes are broken like this.
   1. Try removing hosts from your hostsfile one by one until you find the bad egg

---

# How to use my code...
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
