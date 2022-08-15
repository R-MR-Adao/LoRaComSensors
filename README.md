# LoRaComSensors

<p align="center">
   <img src="https://user-images.githubusercontent.com/111191306/184631638-c912e16f-bdef-4f78-b92b-b682e32e8cef.png">
</p>

Microcontroller-based LoRa communication for remote environmental monitoring. Microcontroller codes to send sensor readings to LoRa gateway: temperature, pressure, humidity, NO2 concentration, and sound level.

https://user-images.githubusercontent.com/111191306/184646263-58941e5d-964d-462f-ac08-a2603f994483.mp4

[See original video](https://mdpi-res.com/sensors/sensors-21-02717/article_deploy/sensors-21-02717-va.mp4) in [MADPI](https://www.mdpi.com/1424-8220/21/8/2717).

## Features
- `*.ino` codes for LoRa microcontrollers:
  - BSFrance LoRa324uII
  - Heltec ESP32 WiFi Lora 32 V2
- Read data from different environmental monitoring sensors:
  - Adafruit BME280 temperature and humidity
  - DFRobot BMP280 temperature pressure
  - FRobot Analog Sound Sensor V2.2
  - GS NO2_968 043 NO2 concentration and temperature
- Establish a communication between a portable microcontroller and a LoRa gateway (LPS8 Dragino)
- Send packets to MQTT sever

## Applications
<p align="center">
   <img src="https://user-images.githubusercontent.com/111191306/184548459-dc975c8f-5005-444d-9dd0-5f75661ded09.png">
</p>

<p align="center">
  TThe age of the Internet of Things (IoT) and smart cities calls for low-power wireless communication networks, for which the Long-Range (LoRa) is a rising star. Efficient network engineering requires the accurate prediction of the Received Signal Strength Indicator (RSSI) spatial distribution. However, the most commonly used models either lack the physical accurateness, resolution, or versatility for cityscape real-world building distribution-based RSSI predictions. For this purpose, we apply the 2D electric field wave-propagation Oscillator Finite-Difference Time-Domain (O-FDTD) method, using the complex dielectric permittivity to model reflection and absorption effects by concrete walls and the receiver sensitivity as the threshold to obtain a simulated coverage area in a $600 \times 600$ m<sup>2</sup> square. Further, we report a simple and low-cost method to experimentally determine the signal coverage area based on mapping communication response-time delays. The simulations show a strong building influence on the RSSI, compared against the Free-Space Path (FSPL) model. We obtain a spatial overlap of 84% between the O-FDTD simulated and experimental signal coverage maps. Our proof-of-concept approach is thoroughly discussed compared to previous works, outlining error sources and possible future improvements. O-FDTD is demonstrated to be most promising for both indoors and outdoors applications and presents a powerful tool for IoT and smart city planners (https://www.mdpi.com/1424-8220/21/8/2717).
</p>

## Table of Contents

- [How to setup](#how-to-setup)
  - [System](#system)
  - [Microcontroller wiring](#microcontroller)
  - [Install libraries](#install-libraries)
  - [Node-RED]("node-red)
- [Publications](#publications)
- [Funding](#funding)

<a name="how-to-setup"/>

## How to setup

### System

Even though microncontroller code should be transportable between different microcontrollers (here two were tested), most of the code was developed and optimized for (LoRa32u4 II, BSFrance).
Other implementations may require slight modifications, such as the signal initialization order.
The figure below describes implemented system. More details can be found in [my paper](https://www.mdpi.com/1424-8220/21/8/2717) regarding this work.

<p align="center">
  <img src="https://user-images.githubusercontent.com/111191306/184682481-69d36268-8e66-43f1-a19c-431af8a17d46.png">
</p>

<p align="center">
  Several environment monitoring sensors for temperature (BMP280, DFRobot), humidity (BME280, Adafruit), noise/sound (Analog Sound Sensor v2.2, DFRobot), and NO<sub>2</sub> concentration (DGS-NO2 968-043, Spec Sens) were wired to a microcontroller board (LoRa32u4 II, BSFrance). The board features a LoRa node chip microcontroller (SX1276, AtmegaQR 32u4 3:3 V @8 MHz MCU), with transmitter power +20 dBm, receiver sensitivity between 􀀀136 dBm @ LoRa 125 kHz SF12 293 bps and 􀀀118 dBm @ LoRa 125 kHz SF6 9380 bps, and an operating frequency of 868 MHz and transmission current-consumption of 128 mA. A 5 V portable power bank drives the micro-controller circuit. The LoRa gateway (LPS8, Dragino), with a transmitter power of +20 dBm, was placed indoors (University of Vigo office), while the micro-controller LoRa transceiver was taken outside for signal coverage measurements.
</p>

<a name="microcontroller"/>

### Microcontroller wiring

<p align="center">
  <img src="https://user-images.githubusercontent.com/111191306/184681334-acdb07e1-d5e8-4d00-8996-5cc0dd16a8da.png">
</p>

<p align="center">
Implemented Long Range (LoRa) gateway system. (a) Schematic of the LoRa communication test device equipped with various sensors on an electronic breadboard. (b) Diagram of the communication process.
</p>

<a name="install-libraries"/>

### Install libraries

The code was implemented, compiled and transfer to the microcontrollers using the Arduino IDE. The required Arduino libraries can be found in the `external` folder in this repository. The can also be installed directly from the Arduino IDE.

### Node-RED

The communication between the LoRa micrcontrollers and the gateway can be implemented in a number of ways. One possibility to quickly represent the data monitoring results is to setup a Node-RED server.
This implementation is not part of this repository, but can be summarized as follows:

Node-RED is a flow-based development tool for visual programming developed originally by IBM for wiring together hardware devices. In this work, Node-RED was used to collect, store and display the data sent from the micro-controllers to the MQTT server.
A screenshot of the developed GUI can be found below. There, the user can select up to four measurements from any sensor to be displayed in real-time and plotted over time. Since the plotted data is stored in the memory of the computer hosting Node-RED (a Raspberry Pi in this case), it was found that plotting more than four mea-
surements at once may lead to program lagging. Also, the response time (ping) of the LoRa communication for each micro-controller can be displayed. This feature was crucial for the mapping of the signal coverage across the city.
Th overall flow of the developed Node-RED program is depicted below. The flow is responsible for the measurement monitors, and response time
display.
While Node-RED offers a large set of task-specific nodes, the java-script enabling function node was found to be a practical and versatile option for most operations, thereby reducing the number of nodes used in the flow. In Node-RED, a message (msg) object coming from an MQTT server contains the properties payload, topic, qos,
msgid and retain, however, only the topic and payload are used in this program. In this case, the topic is a string object used to describe the device and physical quantity from which the message originated. The payload can be either a String, an Integer a JSON object, or a Boolean and it carries the essential content of the message. As can be seen below, the MQTT node labeled ”#” is used to fetch all messaged transmitted to the server. The function node labeled "add timestamp and filename”" is used to filter the messages according to their topic and format specific messages to be saved in text files (see `src\Node-Red\add_sensor_timestamp_and_filename.js`).

The GUI is composed of dashboard nodes Gauge, Chart, used for data representation and Drop down menu, Button and Switch, with which the user can interact. Upon user interaction, the dashboard nodes can be used to inject a payload or trigger an action. For instance, the button node labeled "clear" injects a payload containing an empty JSON object into the chart nodes, thus clearing the displayed data.
Using a function node, additional tasks can be carried out upon user interaction. The function nodes labelled "format" are used to select the which topic shall be presented on each Gauge-Chart pair, using global variables showData Gauge1, showData Gauge2, showData Gauge3 and showData Gauge4. Additionally, they are used
to change the format of the Gauge nodes by changing the limits, units and label, according to the selected physical quantities (see `src\Node-Red\format_for_gauge.js`).

Upon the arrival of a new message from the MQTT server, the function nodes labelled "if right topic" are used to display the data according to the set global variables showData Gauge1, showData Gauge2, showData Gauge3 and showData Gauge4 (see `src\Node-Red\select_topic.js`).

<p align="center">
  <img src="https://user-images.githubusercontent.com/111191306/184685106-308a1465-de75-4d03-a99f-a9529c73093a.png">
</p>

<p align="center">
  (top) Node-RED: Implementation of the Graphical User Interface (GUI) for the display of collected data and sensor response time (ping).
  (bottom) Node-RED: Graphical User Interface (GUI) for data monitoring
</p>


## Publications

<a name="ref-1"/>

\[1\] [Ricardo M. R. Adão, Eduardo Balvís, Alivia V. Carpentier, Humberto Michinel, Jana B. Nieder, "Cityscape LoRa Signal Propagation Predicted and Tested Using Real – World Building - Data Based O - FDTD Simulations and Experimental Characterization," Sensors 21(8), 2717 (2021).](https://doi.org/10.3390/s21082717)

## Funding

![image](https://user-images.githubusercontent.com/111191306/184633801-13c5177d-c0af-41aa-ad8f-ba747dab807a.png)

Title: NANOEATERS: Valorization and transfer of NANOtechnologies to EArly adopTERS of the Euroregion Galicia-Norte Portugal.

Project Description

NANOEATERS is a network of Research Centers created with the objective of supporting Euroregional “early adopters” companies in the application of new nanotechnology-based solutions. Companies, Universities and Technology Centers will work together with INL in the definition of new nano-based commercially available products and /or services. The technologies targeted by the project will offer effective responses to the weaknesses detected in the cross border Smart Specialization Strategy (S3) Galicia – Northern Portugal, contributing to major social challenges as Health, Environmental Monitoring, Food Safety, Energy Efficiency or Industry 4.0.

Project mission:

Encourage synergic cooperation between crossborder RTD centers and universities; Improve the connection between RTD centers and companies so that RTD results can be commercially exploited; and increase private investment in Research and Innovation.

Project activities:

1 – NanoEaters Community Building;
2 – NanoEaters Use Cases: Development of 10 nanotechnology based “Use Cases” based on the synergic cooperation among INL, universities and RTD Centers from Galicia, allowing the development of TRL 3-4 nanotechnologies to TRL 7-8;
3 – Call For Experiments: 22 Experiments proposed by SMEs will be selected for the development of new nanotechnology-based products, services and / or markets;
4 – Experiments development: Technological and Business Support will be given to SMEs during the execution of their proposed experiments, enabling the commercial exploitation of the RTD results through an Acceleration Program.

Start Date: 01 January 2017

End Date: 31 December 2019

Type: INTERREG POCTEP

Contract Number: 0181_NANOEATERS_1_EP

Funding Agency: ERDF

Funding Programme: INTERREG V-A España-Portugal (POCTEP) 2014-2020

INL Role: Partner Beneficiary (Participant Contact: Paula Galvão)

Partners:

- GAIN, Axencia Galega de Innovación (Galician Innovation Agency)(Spain)
- INL (Portugal)
- Universidade de Vigo (Spain)
- Universidade de Santiago de Compostela (Spain)
- GRADIANT, Centro Tecnológico de Telecomunicaciones de Galicia (Spain)
- CTAG, Fundación para la promoción de la Innovación, Investigación y Desarrollo Tecnológico en la Industria de Automoción de Galicia (Spain)
- AIMEN, Asociación de Investigación Metalúrgica del Noroeste (Spain)
- FBGS , Fundación Biomédica Galicia Sur (Spain)

Budget Total: €4, 255, 750.69

Budget INL: €1, 471, 852.35

<p align="center">
  <img width=300 src="https://user-images.githubusercontent.com/111191306/184678983-c143912a-86b9-4a7f-a63d-8c09d300d55e.gif">
</p>


