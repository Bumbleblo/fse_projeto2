import re
import random

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


