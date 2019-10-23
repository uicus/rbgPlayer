import sys
import socket
import os
import subprocess
import shutil
from threading import Thread
import queue
import time

class BufferedSocket:
    def __init__(self, s):
        self.current_buffer = bytearray()
        self.working_socket = s
    def extend_buffer(self):
        len_before = len(self.current_buffer)
        chunk = self.working_socket.recv(2048)
        self.current_buffer += chunk
        return len_before, len_before+len(chunk)
    def cut_on_index(self, index):
        to_return = self.current_buffer[:index]
        self.current_buffer = self.current_buffer[index+1:]
        return to_return
    def read_until(self, byte):
        for i, b in enumerate(self.current_buffer):
            if b == byte:
                return self.cut_on_index(i)
        while True:
            search_begin, search_end = self.extend_buffer()
            if search_begin == search_end:
                return self.cut_on_index(search_end)
            else:
                for i in range(search_begin, search_end):
                    if self.current_buffer[i] == byte:
                        return self.cut_on_index(i)
    def receive_message(self):
        return self.read_until(0)
    def send_message(self, msg):
        self.working_socket.send(msg+b'\0')
    def shutdown(self):
        self.working_socket.shutdown(socket.SHUT_RDWR)

gen_directory = "gen"
gen_inc_directory = gen_directory+"/inc"
gen_src_directory = gen_directory+"/src"
game_name = "game"
game_path = gen_directory+"/"+game_name+".rbg"
semisplit_players = set(["semisplitFlat"])

if len(sys.argv) != 8:
    print("Usage:",sys.argv[0],"<player-kind> <player-port> <server-address> <server-port> <number-of-threads> <miliseconds-per-move> <simulations-limit>")
    exit()

def get_game_section(game, section):
    game_sections = game.split("#")
    for s in game_sections:
        section_fragments = s.split("=")
        if section_fragments[0].strip() == section:
            return section_fragments[1]

def get_comma_separated_item(text, number):
    split_text = text.split(",")
    return split_text[number].strip()

def get_player_name_from_players_item(item):
    split_item = item.split("(",1)
    return split_item[0].strip()

def extract_player_name(game, player_number):
    players_section = get_game_section(game, "players")
    player_item = get_comma_separated_item(players_section, player_number-1)
    return get_player_name_from_players_item(player_item)

def write_game_to_file(server_socket):
    game = str(server_socket.receive_message(), "utf-8")
    if os.path.exists(gen_directory) and os.path.isdir(gen_directory):
        shutil.rmtree(gen_directory)
    os.makedirs(gen_directory)
    os.makedirs(gen_inc_directory)
    os.makedirs(gen_src_directory)
    with open(game_path, 'w') as out:
        out.write(game + "\n")
    return game

def receive_player_name(server_socket, game):
    player_number = int(str(server_socket.receive_message(), "utf-8"))
    return extract_player_name(game, player_number)

def compile_player(num_of_threads, player_kind):
    if player_kind in semisplit_players:
        subprocess.run(["rbg2cpp/bin/rbg2cpp", "-fsemi-split", "-o", "reasoner", game_path]) # assume description is correct
    else:
        subprocess.run(["rbg2cpp/bin/rbg2cpp", "-o", "reasoner", game_path]) # assume description is correct
    shutil.move("reasoner.cpp", gen_src_directory+"/reasoner.cpp")
    shutil.move("reasoner.hpp", gen_inc_directory+"/reasoner.hpp")
    subprocess.run(["make", "-j"+str(num_of_threads), player_kind]) # again, assume everything is ok

def connect_to_server(server_address, server_port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.connect((server_address, server_port))
    return server_socket

def wait_for_player_connection(player_address, player_port, return_value_queue):
    accept_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    accept_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    accept_socket.bind((player_address, player_port))
    accept_socket.listen(1)
    player_socket, _ = accept_socket.accept()
    return_value_queue.put(player_socket)

def start_player(player_address, player_port, player_name, number_of_threads, player_kind, miliseconds_per_move, simulations_limit):
    workers = max(number_of_threads-1, 1)
    return subprocess.Popen(["bin/"+player_kind, str(player_address), str(player_port), str(player_name), str(workers), str(miliseconds_per_move), str(simulations_limit)])

def start_and_connect_player(player_address, player_port, player_name, number_of_threads, player_kind, miliseconds_per_move, simulations_limit):
    return_value_queue = queue.Queue()
    player_connection_wait = Thread(target = wait_for_player_connection, args = (player_address, player_port, return_value_queue))
    player_connection_wait.start()
    player_process = start_player(player_address, player_port, player_name, number_of_threads, player_kind, miliseconds_per_move, simulations_limit)
    player_connection_wait.join()
    return BufferedSocket(return_value_queue.get()), player_process

def forward_and_log(source_socket, target_socket, log_begin, log_end, role):
    while True:
        data = source_socket.receive_message()
        if len(data) == 0:
            print("Connection to",role,"lost! Exitting...")
            target_socket.shutdown()
            quit()
        human_readable = data
        print(log_begin, human_readable, log_end)
        target_socket.send_message(data)

player_address = "127.0.0.1"
player_kind = sys.argv[1]
player_port = int(sys.argv[2])
server_address = sys.argv[3]
server_port = int(sys.argv[4])
number_of_threads = int(sys.argv[5])
miliseconds_per_move = int(sys.argv[6])
simulations_limit = int(sys.argv[7])

server_socket = BufferedSocket(connect_to_server(server_address, server_port))
print("Successfully connected to server!")

game = write_game_to_file(server_socket)
print("Game rules written to:",game_path)

compile_player(number_of_threads, player_kind)
print("Player compiled!")
time.sleep(1.) # to give other players time to end compilation

player_name = receive_player_name(server_socket, game)
print("Received player name:",player_name)

player_socket, player_process = start_and_connect_player(player_address, player_port, player_name, number_of_threads, player_kind, miliseconds_per_move, simulations_limit)
print("Player started!")

server_to_client = Thread(target = forward_and_log, args = (server_socket, player_socket, "Server says-->","<--","server"))
client_to_server = Thread(target = forward_and_log, args = (player_socket, server_socket, "Client says-->","<--","client"))
client_to_server.daemon = True
server_to_client.daemon = True
client_to_server.start()
server_to_client.start()
client_to_server.join()
server_to_client.join()
player_process.terminate()
