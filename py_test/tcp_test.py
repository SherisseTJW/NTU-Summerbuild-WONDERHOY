import socket
import threading

def handle_client(client_socket, addr):
    """
    Handles a single client connection.
    Sends 'hello' and then closes the connection.
    """
    print(f"[*] Accepted connection from: {addr[0]}:{addr[1]}")
    try:
        # Send the "hello" string as bytes
        client_socket.sendall(b"hello\n")
        print(f"[*] Sent 'hello' to {addr[0]}:{addr[1]}")
    except Exception as e:
        print(f"[-] Error sending data to {addr[0]}:{addr[1]}: {e}")
    finally:
        # Close the client socket
        client_socket.close()
        print(f"[*] Connection from {addr[0]}:{addr[1]} closed.")

def run_server(host, port):
    """
    Runs the TCP server, listening for incoming connections.
    """
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # Allows immediate reuse of the address

    try:
        server.bind((host, port))
        server.listen(5) # Max 5 queued connections
        print(f"[*] Listening on {host}:{port}")

        while True:
            client, addr = server.accept()
            # Create a new thread to handle the client connection
            client_handler = threading.Thread(target=handle_client, args=(client, addr))
            client_handler.start()

    except Exception as e:
        print(f"[-] Server error: {e}")
    finally:
        server.close()
        print("[*] Server shut down.")

if __name__ == "__main__":
    HOST = '0.0.0.0'  # Listen on all available network interfaces
    PORT = 12345      # Choose an available port

    run_server(HOST, PORT)
