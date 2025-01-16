# Azure IoT Hub Terraform Script

## Prerequisites

Before running this script, ensure you have the following:

1. **Terraform Installed**: Download and install Terraform from [Terraform's official website](https://www.terraform.io/downloads.html).
2. **Azure CLI Installed**: Install Azure CLI from [Azure CLI installation guide](https://learn.microsoft.com/en-us/cli/azure/install-azure-cli).
3. **Azure Account**: Ensure you have an active Azure subscription.
4. **Logged into Azure CLI**: Log in using the Azure CLI:
   ```bash
   az login
   ```

## Steps to Run the Script

1. **Clone the Repository**:
   Clone this repository or save the `azure_iothub.tf` file to your local machine.

2. **Initialize Terraform**:
   Navigate to the directory containing the `azure_iothub.tf` file and run:
   ```bash
   terraform init
   ```

3. **Review the Terraform Plan**:
   Preview the changes Terraform will make to your Azure account:
   ```bash
   terraform plan
   ```

4. **Apply the Terraform Script**:
   Apply the script to create the resources:
   ```bash
   terraform apply
   ```
   Confirm the prompt by typing `yes`.

5. **View the Outputs**:
   After the resources are created, Terraform will output the connection strings for the IoT Hub and the device. These can be used to connect and interact with the IoT Hub and device.

## Outputs

- **IoT Hub Connection String**: This is used to manage and interact with the IoT Hub.
- **IoT Device Connection String**: This is used to send data to and receive data from the IoT Hub.

Example output:
```bash
Outputs:

iot_hub_connection_string = "<IoT Hub Connection String>"
iot_device_connection_string = "<IoT Device Connection String>"
```

## Clean Up

To remove all resources created by this script, run:
```bash
terraform destroy
```
Confirm the prompt by typing `yes`.

## Notes

- Modify the `location` and resource names in the script as needed.
- Ensure you have the necessary permissions to create resources in your Azure subscription.

