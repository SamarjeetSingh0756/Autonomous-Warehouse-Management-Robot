# flask_robot_control.py
from flask import Flask, jsonify, request, render_template, Response, send_file
import os
import io
from datetime import datetime
import base64
from PIL import Image
import threading
import time
app = Flask(__name__)
# Global states
mode = "AUTO" # or "MANUAL"
manual_command = "stop" # start/stop/left/right
emergency = False
pause_requested = False
# Camera related variables
latest_image = None
latest_image_time = None
image_lock = threading.Lock()
@app.route("/")
def home():
return render_template("index.html", mode=mode, command=manual_command,
emergency=emergency)
@app.route("/status")
def get_status():
global mode, emergency, pause_requested
if emergency:
return "emergency"
if pause_requested:
return "pause"
return mode.lower()
@app.route("/manual_command")
def get_manual_command():
global manual_command
return manual_command
@app.route("/set_mode", methods=["POST"])
def set_mode():
global mode
mode = request.form.get("mode", "AUTO")
return ("", 204)
@app.route("/set_command", methods=["POST"])
def set_command():
global manual_command
manual_command = request.form.get("command", "stop")
return ("", 204)
@app.route("/emergency", methods=["POST"])
def toggle_emergency():
global emergency
emergency = not emergency
return ("", 204)
@app.route("/pause_notice")
def pause_notice():
global pause_requested
pause_requested = True
return ("Pause acknowledged", 200)
@app.route("/approve_resume", methods=["POST"])
def approve_resume():
global pause_requested
pause_requested = False
return ("", 204)
@app.route("/upload_image", methods=["POST"])
def upload_image():
global latest_image, latest_image_time
# Get the raw image data from the request
image_data = request.data
# Save the image with timestamp
with image_lock:
latest_image = image_data
latest_image_time = datetime.now()
return "Image received", 200
@app.route("/camera_feed")
def camera_feed():
global latest_image
if latest_image is None:
# Return a placeholder image or message
return "No image available", 404
with image_lock:
# Return the latest image
return Response(latest_image, mimetype="image/jpeg")
@app.route("/camera_status")
def camera_status():
global latest_image_time
if latest_image_time is None:
return jsonify({
"status": "No image received",
"last_update": None
})
time_diff = (datetime.now() - latest_image_time).total_seconds()
return jsonify({
"status": "Active" if time_diff < 5 else "Inactive",
"last_update": latest_image_time.strftime("%H:%M:%S"),
"seconds_ago": round(time_diff)
})
@app.route("/video_feed")
def video_feed():
def generate():
global latest_image
while True:
with image_lock:
if latest_image is not None:
yield (b'--frame\r\n'
b'Content-Type: image/jpeg\r\n\r\n' + latest_image + b'\r\n')
time.sleep(0.1) # Adjust based on desired frame rate
return Response(generate(), mimetype='multipart/x-mixed-replace; boundary=frame')
if __name__ == "__main__":
app.run(host="0.0.0.0", port=5000, debug=False, threaded=true)