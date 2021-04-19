import re
import socket
import logging
import random

from csv_logger import CSVFormatter
from time import sleep


# logger 
logging.basicConfig(level=logging.DEBUG)

handler = logging.FileHandler('report.csv', 'a+')
handler.setFormatter(CSVFormatter())

logger = logging.getLogger(__name__)
logger.addHandler(handler)


# parse command

command_regex = r'(?P<command>^\w+)\=(?P<args>[^$]+)'

def parse_commands(string: str) -> [str, [str]]:

    command = re.findall(command_regex, string)

    command, args = command[0]
    return command, args.split(':')

# possible messages sended to distribuited server
def gen_message():

    while True:
        command = random.choice(['AR', 'LAMP'])

        if command == 'AR':
            index = random.randint(1,2)
        else:
            index = random.randint(1,4)

        value = random.randint(0, 1)

        yield f'{command} {index} {value}'




if __name__ == '__main__':

    generator = gen_message()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:

        # setting host
        server_socket.bind(('0.0.0.0', 8000))
        server_socket.listen(20)

        logger.info("Server iniciado, esperando conexões")

        # create connection
        while True:

            conn, addr = server_socket.accept()
            logger.info(f'Connected: {addr}')

            while True:

                try:
                    message = next(generator)
                    message += (50 - len(message))*' '

                    logger.info(f'Enviando mensagem: {message.strip()}')

                    data = conn.send(bytes(message, 'utf-8'))

                    data = conn.recv(50)

                    if data:

                        data = data.decode('latin-1')
                        command, args = parse_commands(data)

                        if command == 'SENSORDATA': 
                            logger.info(
                                f'Informações BM280 - temperature {args[0]} e humidity {args[1]}'
                            )
                        elif command == 'EVENT':
                            logger.info(
                                f'Evento recebido do pino: {args[0].strip()}'
                            )

                except (BrokenPipeError, ConnectionResetError):
                    logger.info('Connection closed')
                    break

        # I think that the with clause handle this...
        server_socket.close()
