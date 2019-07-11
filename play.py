import sys
import socket
import os
import subprocess
import shutil
from threading import Thread
import queue

gen_directory = "gen"
game_name = "game"
game_path = gen_directory+"/"+game_name+".rbg"

if len(sys.argv) != 5:
    print("Usage:",sys.argv[0],"<player-port> <server-address> <server-port> <number-of-threads>")
    exit()

def receive_message(source_socket):
    header = source_socket.recv(5)
    length = int(header)
    return str(source_socket.recv(length), "utf-8")[:-1]

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
    player_item = get_comma_separated_item(players_section, player_number)
    return get_player_name_from_players_item(player_item)

def write_game_to_file(server_socket):
    game = receive_message(server_socket)
    if os.path.exists(gen_directory) and os.path.isdir(gen_directory):
        shutil.rmtree(gen_directory)
    os.makedirs(gen_directory)
    with open(game_path, 'w') as out:
        out.write(game + "\n")
    return game

def receive_player_name(server_socket, game):
    player_number = int(receive_message(server_socket))
    return extract_player_name(game, player_number)

def compile_player(num_of_threads):
    subprocess.run(["rbg2cpp/bin/rbg2cpp", "-o", "reasoner", game_path]) # assume description is correct
    shutil.move("reasoner.cpp", gen_directory+"/reasoner.cpp")
    shutil.move("reasoner.hpp", gen_directory+"/reasoner.hpp")
    subprocess.run(["make", "-j"+str(num_of_threads)]) # again, assume everything is ok

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

def start_player(player_address, player_port, player_name, number_of_threads):
    workers = max(number_of_threads-1, 1)
    return subprocess.Popen(["bin/rbgPlayer", str(player_address), str(player_port), str(player_name), str(workers)])

def start_and_connect_player(player_address, player_port, player_name, number_of_threads):
    return_value_queue = queue.Queue()
    player_connection_wait = Thread(target = wait_for_player_connection, args = (player_address, player_port, return_value_queue))
    player_connection_wait.start()
    player_process = start_player(player_address, player_port, player_name, number_of_threads)
    player_connection_wait.join()
    return return_value_queue.get(), player_process

def forward_and_log(source_socket, target_socket, log_begin, log_end, role):
    while True:
        data = source_socket.recv(2048)
        if len(data) == 0:
            print("Connection to",role,"lost! Exitting...")
            quit()
        human_readable = data
        print(log_begin, human_readable, log_end)
        target_socket.send(data)

player_address = "127.0.0.1"
player_port = int(sys.argv[1])
server_address = sys.argv[2]
server_port = int(sys.argv[3])
number_of_threads = int(sys.argv[4])

server_socket = connect_to_server(server_address, server_port)
print("Successfully connected to server!")

game = write_game_to_file(server_socket)
print("Game rules written to:",game_path)

compile_player(number_of_threads)
print("Player compiled!")

player_name = receive_player_name(server_socket, game)
print("Received player name:",player_name)

player_socket, player_process = start_and_connect_player(player_address, player_port, player_name, number_of_threads)
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
