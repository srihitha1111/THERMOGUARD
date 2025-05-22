from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

# Temporary in-memory storage (for development/testing)
temperature_data = []

@app.route("/")
def home():
    return "ThermoGuard Backend Running!"

@app.route("/submit-data", methods=["POST"])
def submit_data():
    data = request.get_json()

    if not data:
        return jsonify({"error": "No data received"}), 400

    temperature_data.append(data)
    print("Data received:", data)  # For terminal log
    return jsonify({"message": "Data saved"}), 200

@app.route("/get-data", methods=["GET"])
def get_data():
    return jsonify(temperature_data), 200

if __name__ == "_main_":
    app.run(debug=True)