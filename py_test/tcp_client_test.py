import socket

def run_client(host, port, message_to_send="Hello from Python Client!"):
    """
    Connects to a TCP server, sends a message, and prints the response.
    """
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        print(f"[*] Attempting to connect to {host}:{port}...")
        client_socket.connect((host, port))
        print(f"[*] Successfully connected to {host}:{port}")

        # Send data
        # Encode the string message to bytes before sending
        client_socket.sendall(message_to_send.encode('utf-8'))
        print(f"[*] Sent: '{message_to_send}'")

        # Receive data from the server
        # The buffer size 1024 is arbitrary, it's the max amount of data to receive at once
        response = client_socket.recv(1024)
        # Decode the received bytes back to a string
        print(f"[*] Received: '{response.decode('utf-8').strip()}'")

    except ConnectionRefusedError:
        print(f"[-] Connection refused. Make sure the server is running on {host}:{port}.")
    except Exception as e:
        print(f"[-] An error occurred: {e}")
    finally:
        # Close the connection
        client_socket.close()
        print("[*] Connection closed.")

if __name__ == "__main__":
    SERVER_HOST = '127.0.0.1'  # The server's IP address (localhost)
    SERVER_PORT = 12345        # The port the server is listening on

    # You can change the message here if you want
    TEST_MESSAGE = "Hello from Python Client!"

    run_client(SERVER_HOST, SERVER_PORT, TEST_MESSAGE)
