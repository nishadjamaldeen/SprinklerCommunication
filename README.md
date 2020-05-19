# Sprinkler Communication

## Project Description

The main goal here is to be able to use both user-generated and automated commands of a specific format and have them be communicated to a number of communication-enabled devices across a network. The system uses a hybrid of a star and mesh network. The star pattern influence comes from the fact that all communication starts at a singular central device. The mesh network capabilities are explored as devices that are further away from central devices will signal hop via nearer devices to reach their target destination. 

The original system utilized ZigBee communication protocols (using XBee Series 2 communication modules), while a later adaption uses LoRa communication protocols (using an RFM95 communication module). 

## Flow of Commands

![Communication Flow](AndromedaCommsFlowChart.jpg)


