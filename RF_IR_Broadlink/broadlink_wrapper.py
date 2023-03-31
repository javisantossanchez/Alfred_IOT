import argparse
import time
import json
from broadlink import rm, rm4, exceptions
from broadlink.exceptions import ReadError, StorageError
TIMEOUT = 30

def get_command(key, filename):
    with open(filename, "r") as f:
        for line in f:
            if key in line:
                return line.strip().split("=")[1]
    return None

def learn_command(ip_address, port, timeout):
    device = rm4((ip_address, port), "EC0BAEEE13FF", devtype=0x610D)
    # Enter learning mode
    device.auth()
    device.enter_learning()

    start = time.time()
    while time.time() - start < timeout:
        try:
            # Check for data
            data = device.check_data()
            if data:
                hex_data = ''.join(format(x, '02x') for x in data)
                print(f"Command learned: {hex_data}")
                break
        except (ReadError, StorageError):
            pass
        time.sleep(1)

#Funcion para enviar comando, diferenciando entre RF o IR con una lista que sirve de historico de los comandos enviados
def send_command(ip_addr, port, timeout, command, commands_history):
    try:
        device = rm4((ip_addr, port), "EC0BAEEE13FF", devtype=0x610D)

        # Enter learning mode
        device.auth()
        command_bytes = bytes.fromhex(command)

        device.send_data(command_bytes)
        commands_history.append(command_bytes)
    except (exceptions.NetworkTimeout, exceptions.AuthorizationError):
        print("Could not connect to device")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Send RF/IR commands to Broadlink devices")
    parser.add_argument("ip_address", type=str, help="IP address of the device")
    parser.add_argument("port", type=int, help="Port number of the device")
    parser.add_argument("timeout", type=int, help="Timeout value in seconds")
    parser.add_argument("key", type=str, help="Key to lookup in the command file")
    parser.add_argument("filename", type=str, help="Name of the command file")
    args = parser.parse_args()
    
    command = get_command(args.key, args.filename)
    if command is None:
        print(f"Error: Command '{args.key}' not found in file '{args.filename}'")
    else:
        commands_history=[]
        send_command(args.ip_address, args.port, args.timeout, command, commands_history)