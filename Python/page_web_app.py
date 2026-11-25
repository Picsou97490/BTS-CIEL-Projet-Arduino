from flask import Flask, render_template, jsonify
import serial
import threading
import time

app = Flask(__name__)

# Configuration de la liaison Arduino
arduino = serial.Serial("COM5", 9600, timeout=1)
time.sleep(2)

etat_led = "INCONNU"
distance = None

# Reception des donnees Arduino
def lecture_arduino():
    global etat_led, distance

    while True:
        ligne = arduino.readline().decode(
            "utf-8", errors="ignore"
        ).strip()

        if ligne == "LED:ON":
            etat_led = "ALLUMEE"

        elif ligne == "LED:OFF":
            etat_led = "ETEINTE"

        elif ligne.startswith("DIST:"):
            try:
                distance = int(ligne.split(":")[1])
            except ValueError:
                pass

# Page principale
@app.route("/")
def accueil():
    return render_template("index.html")

# Allumer la LED
@app.route("/on", methods=["POST"])
def allumer():
    arduino.write(b"1")
    return jsonify({"commande": "ON"})

# Eteindre la LED
@app.route("/off", methods=["POST"])
def eteindre():
    arduino.write(b"0")
    return jsonify({"commande": "OFF"})

# Recuperation des informations
@app.route("/data")
def donnees():
    return jsonify({
        "led": etat_led,
        "distance": distance
    })

# Demarrage du serveur
if __name__ == "__main__":
    threading.Thread(
        target=lecture_arduino,
        daemon=True
    ).start()

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=False
    )
