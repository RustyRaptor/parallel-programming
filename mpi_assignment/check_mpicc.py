import argparse
import paramiko

parser = argparse.ArgumentParser(description='Connect to a list of hosts using SSH')
parser.add_argument('host_file', type=str, help='Path to file containing list of hosts')
parser.add_argument('username', type=str, help='Username for SSH connection')
parser.add_argument('password', type=str, help='Password for SSH connection')
parser.add_argument('output_file', type=str, help='Path to output file')

args = parser.parse_args()

with open(args.host_file, 'r') as f:
        hosts = f.readlines()
        for host in hosts:
                print("Checking host: " + host.strip())
                try:
                        ssh = paramiko.SSHClient()
                        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
                        ssh.connect(hostname=host.strip(), username=args.username, password=args.password)
                except:
                        continue
                stdin, stdout, stderr = ssh.exec_command('/usr/lib64/mpi/gcc/openmpi4/bin/mpicc --version')
                output = stdout.read().decode('utf-8')
                error_output = stderr.read().decode('utf-8')  # Read from stderr
                if 'gcc (SUSE Linux) 7.5.0' not in output:
                        print('does not have mpicc installed\n')
                else:
                        with open(args.output_file, 'a') as out_file:
                                out_file.write(f'{host.strip()}\n')
                ssh.close()
                
        ssh.close()

