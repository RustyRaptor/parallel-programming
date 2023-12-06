import paramiko
import os
# Read the input file with the list of hosts, username, and password
import argparse
import paramiko

parser = argparse.ArgumentParser(
    description="Configure passwordless ssh for remote hosts"
)
parser.add_argument(
    "input_file", type=str, help="path to input file with list of hosts"
)
parser.add_argument("username", type=str, help="username for remote hosts")
parser.add_argument("password", type=str, help="password for remote hosts")

args = parser.parse_args()

with open(args.input_file, "r") as f:
    hosts = [line.strip().split()[0] for line in f]
print(hosts)
# For each host in the list, configure ssh key for passwordless ssh
for host in hosts:
    # Create a new SSH client
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

    # Connect to the remote host
    client.connect(host, username=args.username, password=args.password)
    print("Connected to", host)
    # Generate an ssh key pair on the remote host
    stdin, stdout, stderr = client.exec_command(
        'echo -e "y\n" | ssh-keygen -q -t rsa -N "" -f ~/.ssh/id_rsa'
    )

    print("Generated ssh key pair on", host)
    stdout.channel.recv_exit_status()

    print("Copying public key to authorized_keys on", host)
    # Copy the public key to the remote host's authorized_keys file
    stdin, stdout, stderr = client.exec_command(
        "cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys"
    )
    print("Copied public key to authorized_keys on", host)
    stdout.channel.recv_exit_status()

    # Disconnect from the remote host
    client.close()

    print("Disconnected from", host)

    # Test the ssh connection to the remote host
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(host, username=args.username, key_filename=os.path.expanduser("~/.ssh/id_rsa"))
    stdin, stdout, stderr = client.exec_command("echo success")
    if stdout.read().decode().strip() == "success":
        print(f"Successfully configured passwordless ssh for {host}")
    else:
        print(f"Error configuring passwordless ssh for {host}")
    client.close()
