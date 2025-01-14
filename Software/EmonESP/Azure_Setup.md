# Azure IoT Hub and Device Setup Guide

This guide provides step-by-step instructions on how to create an IoT Hub in Azure, register an IoT device, and retrieve its credentials.

---

## Prerequisites
- An active Azure account. If you don’t have one, [sign up here](https://azure.microsoft.com/free/).
- Basic familiarity with Azure Portal.

---

## Steps to Create an IoT Hub

### 1. Log in to the Azure Portal
1. Go to [Azure Portal](https://portal.azure.com/).
2. Log in with your Azure account credentials.

### 2. Create an IoT Hub
1. In the search bar, type **IoT Hub** and select it.
2. Click **+ Create**.
3. Fill in the required details:
   - **Subscription**: Select your Azure subscription.
   - **Resource Group**: Create a new one or use an existing one.
   - **Region**: Choose the closest region to your location for better latency.
   - **IoT Hub Name**: Enter a unique name for your IoT Hub.
4. Click **Next: Networking**, and configure the networking settings as needed.
5. Proceed through the tabs (Management, Tags) as needed, then click **Review + Create**.
6. Click **Create** after validation.

---

## Steps to Register an IoT Device

### 1. Navigate to the IoT Hub
1. Go to your IoT Hub in the Azure Portal.
2. In the left-hand menu, select **Devices** under the **Explorers** section.

### 2. Create a Device
1. Click **+ New Device**.
2. Provide a **Device ID** (e.g., `my-iot-device`).
3. Choose an authentication type:
   - **Symmetric Key** (default and recommended for simplicity).
   - **X.509 Certificate** (if required for enhanced security).
   - **TPM** (if using device provisioning with TPM).
4. Click **Save** to create the device.

---

## Steps to Retrieve Device Credentials

### 1. Access the Device
1. In the **Devices** section of your IoT Hub, select the device you created.

### 2. View Connection String
1. In the device details, locate the **Primary Connection String** and **Secondary Connection String**.
2. Copy the connection string(s) for later use. 

   Example connection string format:

- `HostName`: The IoT Hub’s hostname.
- `DeviceId`: The ID of your registered device.
- `SharedAccessKey`: The primary or secondary key for the device.

---


### 3. Generate Device Credentials
1. Open Azure Terminal
2. Enter Command ```az login```
3. Enter Command ```az iot hub generate-sas-token --device-id ESP_EMON --hub-name EMONESPHUB1``` to generate shared key.
4. Copy the shared key starting from ```SharedAccessSignature ```
5. This key will be used as password.
6. Username is ```hub_name.azure-devices.net/device_id/?api-version=2021-04-12``` Replace hub_name and device_id.
7. Host Name is ```hub_name.azure-devices.net``` Replcae hub_name.
8. Post is ```8883```
## Next Steps

- Use the connection string in your IoT application to connect to Azure IoT Hub.
- For testing, consider using the Azure IoT Explorer tool to send messages to and from the IoT Hub.

---

## Resources
- [Azure IoT Hub Documentation](https://learn.microsoft.com/azure/iot-hub/)
- [Azure IoT Explorer](https://github.com/Azure/azure-iot-explorer)
- [Azure Free Account Setup](https://azure.microsoft.com/free/)

---

## Notes
- Ensure you securely store your connection strings and keys. Avoid hardcoding them into your application.
- Rotate device keys periodically to maintain security.

