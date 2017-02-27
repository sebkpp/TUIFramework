import socket
import json

input("Press ENTER when the Python Interface is ready")

with open("../TUIdict.json") as json_file:
    jsonDict = json.load(json_file)
print("Configuration from JSON file loaded")
print(jsonDict)

hote = "localhost"
port = 12800

connexion_avec_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connexion_avec_serveur.connect((hote, port))
print("Connection established on the port {}".format(port))

while True:
    msg_a_envoyer = "Size"

    msg_a_envoyer = msg_a_envoyer.encode()

    connexion_avec_serveur.send(msg_a_envoyer)
    msg_recu = connexion_avec_serveur.recv(1024)
    print(msg_recu.decode())
    byteSize = int(msg_recu.decode())

    msg_a_envoyer = "Received"

    msg_a_envoyer = msg_a_envoyer.encode()

    connexion_avec_serveur.send(msg_a_envoyer)

    msg_recu = connexion_avec_serveur.recv(byteSize)

    jsonLoad = json.loads(msg_recu.decode())
    print(jsonLoad)
