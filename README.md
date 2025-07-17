<h1 align="center">Autonomous Warehouse Management Robot</h1>

<p align="center">A Mobile robot designed for Automating Warehouse Operations.</p>

<hr>

<h2>Author</h2>
<p>Created by <strong>Samarjeet Singh</strong> and <strong>Vasu Jindal</strong></p>

<hr>

<h2>Overview</h2>
<p>
This project focuses on developing an intelligent mobile robot designed to automate warehouse operations such as transporting boxes or items from one location to another. This robot aims to reduce human labor, minimize errors, and improve operational speed by navigating the warehouse autonomously, identifying cargo, and delivering it to designated spots.
</p>

<h2>Key Functionalities</h2>
<ul>
  <li>
    <strong>Autonomous Navigation:</strong>
    The robot moves around the warehouse using sensors and logic. It detects obstacles, maps its path, and reaches destinations on its own.
  </li>
  <li>
    <strong>Inventory Movement:</strong>
    Picks up items and delivers them to locations like storage shelves, packing stations, or delivery zones.
  </li>
  <li>
    <strong>Task Execution Based on Commands:</strong>
    The robot works based on wireless commands or follows pre-set routines.
  </li>
  <li>
    <strong>Error Handling and Alerts:</strong>
    If the robot encounters a blocked path or delivery issue, it pauses and alerts for human intervention.
  </li>
  <li>
    <strong>RFID-based Cargo Management:</strong>
    On startup, the RFID module is activated. Warehouse staff scan cargo tags. The ESP8266 creates a delivery plan and logs data like source, destination, cargo type, and time in an Excel file.
  </li>
  <li>
    <strong>Color-Code Navigation:</strong>
    After mapping, the robot waits for a start command. It follows colored floor paths:
    <ul>
      <li>🟢Green - Turn right</li>
      <li>🟡Yellow - Turn left</li>
      <li>🔴Red - Stop immediately</li>
      <li>🔵Blue - Pause and wait for input</li>
    </ul>
    The ESP32-CAM handles movement decisions using the color sensor and also streams a live video feed for monitoring.
  </li>
  <li>
    <strong>Cargo Drop-off & Return:</strong>
    The robot halts at delivery points. Workers unload the cargo, then the robot resumes or returns to base after completing all tasks.
  </li>
  <li>
    <strong>Manual Mode (Optional):</strong>
    For special tasks or troubleshooting, the robot can be manually operated.
  </li>
</ul>

<h2>Components Used</h2>
<ul>
  <li>Lithium Cells (x7)</li>
  <li>150 RPM DC Metal Geared Motors (x2)</li>
  <li>Servo Motor (for Steering)</li>
  <li>ESP32-CAM Module</li>
  <li>ESP8266 Module</li>
  <li>Color Sensor (TCS34725)</li>
  <li>Voltage Regulator</li>
  <li>Buzzer</li>
  <li>L298N Motor Driver Module</li>
  <li>Battery Management System (BMS)</li>
  <li>RFID Module</li>
</ul>

<hr>

<p align="center">Feel free to fork, modify, and contribute to this project.</p>
