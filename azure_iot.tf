provider "azurerm" {
  features {}
}

resource "azurerm_resource_group" "iot_rg" {
  name     = "iot-hub-resource-group"
  location = "East US"
}

resource "azurerm_iothub" "iot_hub" {
  name                = "my-iot-hub"
  location            = azurerm_resource_group.iot_rg.location
  resource_group_name = azurerm_resource_group.iot_rg.name
  sku {
    name     = "S1"
    tier     = "Standard"
    capacity = 1
  }

  tags = {
    environment = "Terraform"
  }
}

resource "azurerm_iothub_device" "iot_device" {
  name                = "my-iot-device"
  iothub_name         = azurerm_iothub.iot_hub.name
  resource_group_name = azurerm_resource_group.iot_rg.name
}

output "iot_hub_connection_string" {
  value = azurerm_iothub.iot_hub.primary_connection_string
}

output "iot_device_connection_string" {
  value = azurerm_iothub_device.iot_device.connection_string
}
