import argparse
from broadlink import rm, rm4, exceptions

#Funcion para enviar comando, diferenciando entre RF o IR con una lista que sirve de historico de los comandos enviados
def send_command(ip_addr, port, timeout, command, commands_history):
    try:
        device = rm((ip_addr, port), None)
        device.auth()
        device.send_data(command)
        packet_length = len(command)
        if packet_length <= 100:
            print("IR command sent successfully!")
        else:
            print("RF command sent successfully!")
        commands_history.append(command)
    except (exceptions.NetworkTimeout, exceptions.AuthorizationError):
        print("Could not connect to device")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Send RF/IR commands to Broadlink devices")
    parser.add_argument("ip_address", type=str, help="IP address of the device")
    parser.add_argument("port", type=int, help="Port number of the device")
    parser.add_argument("timeout", type=int, help="Timeout value in seconds")
    parser.add_argument("command", type=str, help="Command to send to the device")
    args = parser.parse_args()

    send_command(args.ip_address, args.port, args.timeout, args.command)
    
#Ejemplo de ejecucion: python broadlink_wrapper.py 192.168.1.100 80 5 ABCDEF0123456789


#Funcion para aprender comando
def learn_command(ip_addr, port, timeout):
    try:
        device = rm4((ip_addr, port), None)
        device.auth()
        device.enter_learning()
        print("Learning mode entered. Press the button on the remote to learn the command.")
        ir_packet = None
        while ir_packet is None:
            ir_packet = device.check_data()
        hex_packet = ir_packet.hex()
        print(f"Command learned: {hex_packet}")
        return hex_packet
    except (exceptions.NetworkTimeout, exceptions.AuthorizationError):
        print("Could not connect to device")
        return None

#Ejemplo de ejecucion: python broadlink_wrapper.py 192.168.1.100 80 5
