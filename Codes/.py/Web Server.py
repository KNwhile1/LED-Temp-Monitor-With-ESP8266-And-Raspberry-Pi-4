# =====================================================================================================================
# Name: Web Server.py
# Description: This program creates a web interface using Flask to control an LED and retrieve temperature data from an ESP8266 device.
# Compiler: Geany
# Usage: To control a LED connected to an ESP8266 and collect ambient temperature
# Restriction(s): None.
# History: 05/13/2024 | E. Zoukou / C. Courtemanche / K. Niamba / Creation;
#          1/28/2025 | K. Niamba / Modification ---> Documentation translation (FR to ENG);
# =====================================================================================================================

import requests
from bs4 import BeautifulSoup
from flask import Flask, request, render_template
import pygame

app = Flask(__name__)                                   # Create an instance of the Flask application

ESP8266_IP = "***"                                      # IP address of the ESP

@app.route('/')                                         # Defines a route for the root URL that calls the index() function
def index():
    return render_template('Control Page.html')         # Name of the HTML file
  
                                                        # Sends an HTTP request to the ESP8266 to get the temperature
@app.route('/temp')
def temp():         
    response = requests.get(f"http://{ESP8266_IP}/temp")
    soup = BeautifulSoup(response.text, 'html.parser')
    temperature = soup.find('p').text.strip('')
    return temperature

@app.route('/led', methods=['POST'])                     # Defines a route for the URL with the POST method
                                                         # Analyzes the POST request and turns the LED on or off based on the command
def control_led():
    if request.method == 'POST':
        action = request.form['action']
        if action == 'on':
            requests.get(f"http://{ESP8266_IP}/led/on")  # Request command to turn on the LED
            return "LED turned on"
        elif action == 'off':
            requests.get(f"http://{ESP8266_IP}/led/off") # Request command to turn off the LED
            return "LED turned off"
    return "Invalid request"
    
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80, debug=True)