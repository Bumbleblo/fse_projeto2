import socket
from time import sleep

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

    s.bind(('0.0.0.0', 8000))
    s.listen(20)

    while True:

        conn, addr = s.accept()
        print(f'Connected: {addr}')

        while True:
            print(f'Sending data')

            try:
                data = conn.send(b'UPLAMP=1')

                data = conn.recv(1024)

                print(data)

            except BrokenPipeError:
                print('Connection closed')
                break

            sleep(1)

    s.close()
