
#include "LibBle.h"

WinBleLib::WinBleLib()
{
    bleWatcher.Received(recieved_callback); // [this](BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {this->didDiscoverPeripheral(watcher, eventArgs); });
    bleWatcher.Stopped(stopped_callback); // [this](BluetoothLEAdvertisementWatcher w, BluetoothLEAdvertisementWatcherStoppedEventArgs b) {this->didCancelScanning(); });
}
WinBleLib::~WinBleLib()
{
    bleWatcher.Stop();

}

void WinBleLib::scan()
{
    bleWatcher.Start();
}

void WinBleLib::stop()
{
    bleWatcher.Stop();
}

void recieved_callback(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs)
{

    if (eventArgs.Advertisement().ServiceUuids().Size() > 0)
    {
        for (auto guid : eventArgs.Advertisement().ServiceUuids())
        {

            if (winrtGuidToString(guid) == "dfaf765b-42d7-4c71-b13e-9262347354bf")
            {
                std::cout << "discovered and connected " << eventArgs.BluetoothAddress() << std::endl;
                connectPeripheral(eventArgs.BluetoothAddress());
                watcher.Stop();
                return;
            }
            else
            {
                std::cout << "didnt connect to: " << winrtGuidToString(guid) << std::endl;
            }
        }
    }
    else
    {
        std::cout << "no services on this device " << eventArgs.BluetoothAddress() << std::endl;
    }

}

void stopped_callback(BluetoothLEAdvertisementWatcher w, BluetoothLEAdvertisementWatcherStoppedEventArgs b)
{
    std::cout << "stop callback" << std::endl;
}

void connectPeripheral(uint64_t windowsDeviceAddress)
{
    BluetoothLEDevice::FromBluetoothAddressAsync(windowsDeviceAddress).Completed(connect_callback);
}

void connect_callback(IAsyncOperation<BluetoothLEDevice> sender, AsyncStatus status)
{
    if (auto device = sender.GetResults(); device)
    {
        if (status == winrt::Windows::Foundation::AsyncStatus::Completed)
        {
            std::cout << "connected " << std::endl;
            discoverServices(device);
            //this->didConnectPeripheral(device);
        }
        else
        {
            std::cout << "not connected " << std::endl;
        }
    }
    else
    {
        std::cout << "Device is Null: " << sender.ErrorCode() << std::endl;
    }
}

void discoverServices(BluetoothLEDevice& device)
{
    std::wcout << "didConnectPeripheral:" << device.Name().c_str() << std::endl;
    device.GetGattServicesAsync().Completed(services_callback);

}

void services_callback(IAsyncOperation<GattDeviceServicesResult>sender, AsyncStatus status)
{

    if (GattDeviceServicesResult result = sender.get(); result)
    {
        if (status == winrt::Windows::Foundation::AsyncStatus::Completed)
        {
            std::cout << "service callback" << std::endl;
            service(result.Services(), result.Status());
        }
        else
        {
            std::cout << "not service " << std::endl;
        }
    }
    else
    {
        std::cout << "Device is Null: " << sender.ErrorCode() << std::endl;
    }
}

void service(IVectorView<GattDeviceService> services, GattCommunicationStatus status)
{
    if (status == GattCommunicationStatus::Success)
    {
       // std::cout << "didDiscoverServices: " << services.GetAt(0).Device().Name().c_str() << std::endl;

        for (auto service : services)
        {
            std::cout << "services count\n";
            discover_services(service);
        }
    }
    else
    {
        std::cout << "Error Getting Services: ";
        switch (status)
        {
        case winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::Unreachable:
            std::cout << "Unreachable";
            break;
        case winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::ProtocolError:
            std::cout << "ProtocolError";
            break;
        case winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::AccessDenied:
            std::cout << "AccessDenied";
            break;
        }
        std::cout << std::endl;
    }
}

void discover_services(GattDeviceService service)
{
    service.GetCharacteristicsAsync().Completed(discover_services_callback);
}

void discover_services_callback(IAsyncOperation<GattCharacteristicsResult>sender, AsyncStatus status)
{
    if (GattCharacteristicsResult result = sender.get(); result)
    {
        switch (status)
        {
        case winrt::Windows::Foundation::AsyncStatus::Completed:
            std::cout << "connect to char\n";
            discover_characteristics(result.Characteristics(), result.Status());
            break;
        case winrt::Windows::Foundation::AsyncStatus::Canceled:
        case winrt::Windows::Foundation::AsyncStatus::Error:
        case winrt::Windows::Foundation::AsyncStatus::Started:
            break;
            //error func
        }
    }
    else
    {
        std::cout << "Characteristics are Null" << std::endl;
    }
}

void discover_characteristics(IVectorView<GattCharacteristic> characteristics, GattCommunicationStatus status)
{
    if (status == GattCommunicationStatus::Success)
    {
        std::cout << "didDiscoverCharacteristicsForService: " << winrtGuidToString(characteristics.GetAt(0).Service().Uuid()) << std::endl;

        if (winrtGuidToString(characteristics.GetAt(0).Service().Uuid()) != "dfaf765b-42d7-4c71-b13e-9262347354bf")
        {
            std::cout << "did not connect" << std::endl;
            return;
        }

        for (auto characteristic : characteristics)
        {

            std::cout << "added characteristic " << std::endl;
            SingletonCharacteristics* sc = SingletonCharacteristics::getInstance();
            sc->setChars(characteristic);

            //std::cout << "Characteristic: " << winrtGuidToString(characteristic.Uuid()) << " : " << characteristic.UserDescription().c_str() << std::endl;
            //readValueForCharacteristic(characteristic);

        }
    }
    else
    {
        std::cout << "Error Getting Characteristics: ";
        switch (status)
        {
        case winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::Unreachable:
            std::cout << "Unreachable";
            break;
        case winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::ProtocolError:
            std::cout << "ProtocolError";
            break;
        case winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::AccessDenied:
            std::cout << "AccessDenied";
            break;
        }
        std::cout << std::endl;
    }
}

void subscribe_to_char(GattCharacteristic gattChar)
{
    GattClientCharacteristicConfigurationDescriptorValue cccdValue = GattClientCharacteristicConfigurationDescriptorValue::Notify;
    while (gattChar == NULL)
    {
    }
    gattChar.WriteClientCharacteristicConfigurationDescriptorAsync(cccdValue);
}

void characteristic_value_changed(GattCharacteristic const&, GattValueChangedEventArgs args)
{
    //std::cout << "start" << std::endl;
    //std::cout << "origin: " << args.CharacteristicValue().data()[0] << std::endl;
    char test = args.CharacteristicValue().data()[0];
    //std::cout << "test: " << test << std::endl;
    std::string things{ test };
    //ShowDesktop(things);
    SingletonCharacteristics* sc = SingletonCharacteristics::getInstance();
    if (sc->gui_cb != nullptr)
    {
        sc->gui_cb(&test);
    }
    /*
    for (int i = 0; i < args.CharacteristicValue().Length(); i++)
    {
        std::cout << args.CharacteristicValue().data()[i] << std::endl;
    }
    */


    // BT_Code: An Indicate or Notify reported that the value has changed.
    // Display the new value with a timestamp.

}

std::string winrtGuidToString(winrt::guid uuid)
{
    char uuidCStr[100];
    if (uuid.Data2 == 0)
        sprintf(uuidCStr, "%04x", uuid.Data1);
    else
        sprintf(uuidCStr, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", uuid.Data1, uuid.Data2, uuid.Data3, uuid.Data4[0], uuid.Data4[1], uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
    std::string guid = std::string(uuidCStr);
    return guid;
}

void ShowDesktop(std::string arduinoInpt)
{
    //std::cout << "printing characters " << std::endl;
    //OutputString(L"Sending 'Win-D'\r\n");
    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));

    //inputs[0].type = INPUT_KEYBOARD;
    //inputs[0].ki.wVk = VK_LWIN;

    //inputs[1].type = INPUT_KEYBOARD;
    //inputs[1].ki.wVk = 'D';

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;
    inputs[0].ki.wScan = arduinoInpt.c_str()[0];

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        //std::cout << "printing characters  failed" << std::endl;
        //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
    //std::cout << "end printing characters " << std::endl;
}


void startBLE(std::function<void(char*)> set_gui_cb)
{
    if (set_gui_cb != nullptr)
    {
        set_cb(set_gui_cb);
    }


    WinBleLib winBleLib;
    winBleLib.scan();
    SingletonCharacteristics* sc = SingletonCharacteristics::getInstance();
    for (;;)
    {
        if (sc->getChars().size() != 0)
        {
            break;
        }
    }
    //std::cout << "subscribe to char" << std::endl;
    subscribe_to_char(sc->getChars()[0]);

    sc->getChars()[0].ValueChanged(&characteristic_value_changed);
    while (getchar() != '\n');
    winBleLib.stop();
}

void set_cb(std::function<void(char*)> set_gui_cb)
{
    SingletonCharacteristics* sc = SingletonCharacteristics::getInstance();
    sc->gui_cb = set_gui_cb;
}
