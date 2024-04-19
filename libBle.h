#pragma once
#include "pch.h"
// Windows BLE Interface
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "SingletonSupport.h"
#include <functional>

using winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;
using winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisement;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStoppedEventArgs;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicProperties;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicsResult;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattSession;
using winrt::Windows::Storage::Streams::DataWriter;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattWriteOption;
using winrt::Windows::Devices::Bluetooth::BluetoothCacheMode;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Devices::Bluetooth::GenericAttributeProfile;

class WinBleLib
{
public:
    WinBleLib();
    ~WinBleLib();

    void scan();

    void stop();

private:
    BluetoothLEAdvertisementWatcher bleWatcher;


};

void recieved_callback(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs);

void stopped_callback(BluetoothLEAdvertisementWatcher w, BluetoothLEAdvertisementWatcherStoppedEventArgs b);

void connectPeripheral(uint64_t);

void connect_callback(IAsyncOperation<BluetoothLEDevice> sender, AsyncStatus status);

void discoverServices(BluetoothLEDevice& device);

void services_callback(IAsyncOperation<GattDeviceServicesResult>sender, AsyncStatus status);

void service(IVectorView<GattDeviceService> services, GattCommunicationStatus status);

void discover_services(GattDeviceService service);

void discover_services_callback(IAsyncOperation<GattCharacteristicsResult>sender, AsyncStatus status);

void discover_characteristics(IVectorView<GattCharacteristic> characteristics, GattCommunicationStatus status);

void subscribe_to_char(GattCharacteristic gattChar);

void characteristic_value_changed(GattCharacteristic const&, GattValueChangedEventArgs args);

std::string winrtGuidToString(winrt::guid);

void ShowDesktop(std::string input);

void startBLE(void* context, std::function<void(char*, void*)> set_gui_cb);

void set_cb(std::function<void(char*, void*)> set_gui_cb, void* context);