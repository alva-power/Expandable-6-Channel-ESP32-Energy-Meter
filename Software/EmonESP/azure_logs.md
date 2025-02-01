# Azure IoT Setup and Data Flow

This document outlines the Azure services used for receiving and processing data from an IoT device (e.g., an ESP32) and how these services interact.

## Azure Services Used

The following Azure services are integral to this IoT project:

1.  **IoT Hub:**
    *   **Purpose:**  Acts as a central message broker for IoT devices to connect and send messages to Azure.
    *   **Functionality:** Receives telemetry data sent by devices via MQTT. Manages device identities and connections.
    *   **Relevance:** The primary entry point for device data into the Azure ecosystem.

2.  **Log Analytics Workspace:**
    *   **Purpose:**  A cloud-based service for storing and analyzing logs and telemetry data.
    *   **Functionality:** Ingests telemetry data from the IoT Hub, allowing for querying, analysis, and visualization.
    *   **Relevance:** Used for storing, processing, and deriving insights from the device telemetry data.

3.  **IoT Hub Device Entry:**
    *   **Purpose:**  A representation of your physical IoT device within the IoT Hub.
    *   **Functionality:**  Enables secure device connections and authentication. Assigns a unique identifier to your device.
    *   **Relevance:**  Allows the device to communicate with the IoT Hub in a secure and authenticated way.

## Data Flow

The data flows through the following steps:

1.  **Device to IoT Hub:**
    *   Your IoT device (e.g., an ESP32) publishes MQTT messages to a specific topic in your IoT Hub (e.g., `devices/{deviceId}/messages/events/`).
2.  **IoT Hub Ingestion:**
    *   The IoT Hub receives these MQTT messages.
3.  **Message Routing:**
    *   The IoT Hub routes the incoming messages based on pre-configured settings. These messages can be routed to different endpoints.
4.  **Data Storage and Analysis (Log Analytics):**
    *   Data that is configured to be routed to the Log Analytics Workspace.
    *   The Log Analytics Workspace stores this data, allowing querying via Kusto Query Language (KQL).
    *   The data in this workspace can then be used for analysis and visualization.

## Viewing and Storing Messages

### Viewing Messages in the IoT Hub

*   **Cosmos DB:**
    *   Search `emonespdb` in Azure, and open it.
    *   You will see a containers table.
    *   Click on `emonespmqttmesssages`.
    *   Then click on `emonespmqttdb`, then on `emonespmqttmesssages`, and then on `Items`.
    *   There you will see all MQTT events and messges.
    *   Click on any of them, and you will see the data.
    *   The message is in the `Body` field of data, and it base64 encoded, which needs to be decoded to UTF-8
