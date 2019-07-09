import sys
import socket
import os
from threading import Thread

gen_directory = "gen"
game_name = "game"

if len(sys.argv) != 4:
    print("Usage:",sys.argv[0],"<player-port> <server-address> <server-port>")
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
    if not os.path.exists(gen_directory):
        os.makedirs(gen_directory)
    with open(gen_directory+"/"+game_name+".rbg", 'w') as out:
        out.write(game + "\n")
    return game

def receive_player_name(server_socket, game):
    player_number = int(receive_message(server_socket))
    return extract_player_name(game, player_number)

player_address = "127.0.0.1"
player_port = int(sys.argv[1])
server_address = sys.argv[2]
server_port = int(sys.argv[3])

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.connect((server_address, server_port))
print("Successfully connected to ",server_address+":"+str(server_port))

game = write_game_to_file(server_socket)
print("Game rules written to:",gen_directory+"/"+game_name+".rbg")

player_name = receive_player_name(server_socket, game)
print("Received player name: ",player_name)
