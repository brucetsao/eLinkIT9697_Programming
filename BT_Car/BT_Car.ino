/*
  This example configures LinkIt 7697 to act as a simple GATT server with 1 characteristic.

  To use it, open AppInventor project:

    * 

  Build & install it on Android id

  created Mar 2017
*/
#include <LBLE.h>
#include <LBLEPeriphral.h>

// Define a simple GATT service with only 1 characteristic
LBLEService ledService("0000ffe0-0000-1000-8000-00805f9b34fb");
LBLECharacteristicInt switchCharacteristic("0000FFE1-0000-1000-8000-00805F9B34FB", LBLE_READ | LBLE_WRITE);

void setup() {

  // Initialize LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // Initialize BLE subsystem
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }
  Serial.println("BLE ready");

  Serial.print("Device Address = [");
  Serial.print(LBLE.getDeviceAddress());
  Serial.println("]");

  // configure our advertisement data.
  // In this case, we simply create an advertisement that represents an
  // connectable device with a device name
  LBLEAdvertisementData advertisement;
  advertisement.configAsConnectableDevice("BLE LED");

  // Configure our device's Generic Access Profile's device name
  // Ususally this is the same as the name in the advertisement data.
  LBLEPeripheral.setName("BLE LED");

  // Add characteristics into ledService
  ledService.addAttribute(switchCharacteristic);

  // Add service to GATT server (peripheral)
  LBLEPeripheral.addService(ledService);

  // start the GATT server - it is now 
  // available to connect
  LBLEPeripheral.begin();

  // start advertisment
  LBLEPeripheral.advertise(advertisement);
  for(int i = 6; i< 14; i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
}

void loop() {
  delay(100);
  if (switchCharacteristic.isWritten()) {
    char value = switchCharacteristic.getValue();
    for(int i = 0; i < 4; i++){
      switch(value%4){
        case 1:
          digitalWrite(6+i*2,LOW);
          digitalWrite(7+i*2,HIGH);
          break;
        case 2:
          digitalWrite(7+i*2,LOW);
          digitalWrite(6+i*2,HIGH);
          break;
        default:
          digitalWrite(6+i*2,LOW);
          digitalWrite(7+i*2,LOW);
          break;
      }
      value/=4;
    }
    /*switch (value) {
      case 1:
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      case 0:
        digitalWrite(LED_BUILTIN, LOW);
        break;
      default:
        Serial.println("Unknown value written");
        break;
    }*/
  }
}