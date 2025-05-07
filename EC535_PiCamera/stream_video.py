from flask import Flask, render_template
from threading import Thread
from time import sleep
from picamera2 import Picamera2
import os



# Create Flask app
app = Flask(__name__)
camera = Picamera2()
image_path = 'static/image.jpg'


def capture_loop():
	print("Starting camera setup...") # DEBUG
	camera.configure(camera.create_still_configuration())
	camera.start()
	print("Camera setup complete") # DEBUG
	while (1):
		print("Capturing Image...") # DEBUG
		camera.capture_file(image_path)
		sleep(5)

@app.route('/')
def index():
	return render_template('index.html')


if __name__ == "__main__":
	# Ensure that proper folders and files exists
	if not os.path.exists('static'):
		os.makedirs('static')
		
	#if not os.path.exists(image_path):
		#camera.capture(image_path)
		
	# Start capture thread
	capture_thread = Thread(target=capture_loop, daemon=True)
	capture_thread.start()
	
	# Start Flask
	app.run(host="0.0.0.0", port=5000)
