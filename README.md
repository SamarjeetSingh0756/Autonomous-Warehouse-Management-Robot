<h1 align="center">Warehouse Management Robot using ESP32</h1>

<p align="center">
  <i>Smart robot using ESP-NOW, color recognition & server control</i>
</p>

<hr>

<h2>Author</h2>
<ul>
  <li><b>Name:</b> Samarjeet Singh</li>
  <li><b>Name:</b> Vasu Jindal</li>
</ul>

<h2>Project Overview</h2>
<p>
This is a hardware-based warehouse management robot built using an ESP32 microcontroller. It uses a <b>TCS34725 color sensor</b> to detect ground colors that trigger different behaviors. The robot communicates with another ESP32 via <b>ESP-NOW protocol</b> and interacts with the user through a <b>server-based interface</b>. Movement is powered using <b>L298N motor driver</b>, and feedback is provided using a <b>buzzer</b>.
</p>

<h2>Components Used</h2>
<ul>
  <li>2 × ESP Dev Boards(ESP32 CAM, ESP8266)</li>
  <li>TCS34725 Color Sensor(x1)</li>
  <li>L298N Motor Driver(x1)</li>
  <li>DC Motors(x2)</li>
  <li>Buzzer(x1)</li>
  <li>Voltage regulator(x1)</li>
  <li>Battery Management System(x1)</li>
  
  <li>Jumper wires, battery pack, wheels, chassis, servo motor etc.</li>
</ul>

<h2>Features</h2>
<ul>
  <li>ESP-NOW based peer-to-peer communication between robots</li>
  <li>Color-based triggering of robot actions</li>
  <li>Server-controlled commands for interaction</li>
  <li>Custom behavior based on surface color (e.g., stop, boost, redirect)</li>
</ul>

<h2>How It Works</h2>
<p>
The robot reads the floor color using the TCS34725 sensor and sends this data to another ESP32 via ESP-NOW. The server sends commands to the robot such as movement, halt, or route redirection. The motors respond via the L298N driver and appropriate buzzer feedback is given.
</p>

<h2> Building Guide</h2>
<ol>
  <li><b>Coding Setup:</b><br>
    Open the Arduino IDE → Select your board as <b>ESP32 Dev Module</b> → Select the correct COM port. Begin writing your code in <b>C language</b> for better memory efficiency.
  </li>
  <br>
  <li><b>Debugging:</b><br>
    Compile and test your code. Fix any bugs and optimize the code to minimize memory usage and maximize efficiency.
  </li>
  <br>
  <li><b>Uploading and Wiring:</b><br>
    Once tested, upload the code to the ESP32. Connect all components (motors, sensor, buttons, buzzer).
  </li>
  <br>
  <li><b>Wiring:</b><br>
    <b>Color Sensor (TCS34725):</b>
    <ul>
      <li>VIN → ESP32 3.3V</li>
      <li>GND → ESP32 GND</li>
      <li>SCL → ESP32 D22</li>
      <li>SDA → ESP32 D21</li>
      <li>LED → Leave as is or connect via switch</li>
    </ul>
    <b>L298N Motor Driver:</b>
    <ul>
      <li>IN1 → ESP32 D32</li>
      <li>IN2 → ESP32 D33</li>
      <li>IN3 → ESP32 D25</li>
      <li>IN4 → ESP32 D26</li>
      <li>ENA/ENB (Enable Pins) → ESP32 via PWM capable pins</li>
      <li>VCC → Battery +ve</li>
      <li>GND → Common Ground</li>
    </ul>
    <b>Buzzer:</b>
    <ul>
      <li>+ve → ESP32 D13</li>
      <li>-ve → ESP32 GND</li>
    </ul>
    <b>Power Supply:</b>
    <ul>
      <li>3 × 1.5V AAA batteries (in series)</li>
      <li>+ve → ESP32 VIN</li>
      <li>-ve → ESP32 GND</li>
    </ul>
  </li>
</ol>

<h2>Color Sensor Calibration</h2>
<p>
The TCS34725 sensor is highly sensitive to light, color variation, and distance. Calibrate under consistent conditions to avoid faulty readings.
</p>

<ul>
  <li>Keep the sensor 1–5 cm away from the surface during all readings.</li>
  <li>Use the onboard white LED for consistent lighting, or ensure ambient light is constant.</li>
  <li>Note the RGB values for each surface color (e.g., red → stop, green → boost, black → turn).</li>
  <li>Test on actual warehouse floors for best performance.</li>
</ul>

<h2>⚠️ Precautions</h2>
<ul>
  <li>Ensure <b>all wires are securely connected</b> to prevent intermittent behavior.</li>
  <li>Double-check <b>polarity and orientation</b> of power lines, motors, and sensors.</li>
  <li>Make sure <b>no wires are shorting</b> against each other or the chassis.</li>
  <li>Use a <b>common ground</b> for all modules (motor driver, sensors, ESP32, etc.).</li>
  <li>Keep the power within recommended voltage limits (3.3V or 5V depending on component).</li>
</ul>

<h2>Final Testing</h2>
<p>
Place your robot on a test track with distinct colored paths. Start your server interface and initiate communication with the robot. Observe how it reacts to the floor colors and responds to commands. Make adjustments to color thresholds and logic as needed.
</p>

<hr>

<h2>Credits</h2>
<p>Developed by Samarjeet Singh and Vasu Jindal as part of a final-year engineering project. Inspired by the potential of real-world smart warehouse automation using simple hardware.</p>
