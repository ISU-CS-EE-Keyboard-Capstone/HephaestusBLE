#pragma once
#include "pch.h"

#ifndef SINGLETON_CHARATERISTICS_H
#define SINGLETON_CHARATERISTICS_H

#include <vector>
#include <functional>

using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;

class SingletonCharacteristics {
private:

    static SingletonCharacteristics* instancePtr;
    std::vector<GattCharacteristic> gattChars;
    std::vector<std::string> notConnectedAddresses;
    std::vector<std::string> connectedAddresses;
    
    SingletonCharacteristics()
    {
    }

public:

    std::function<void(char*, void*)> gui_cb = nullptr;
    void* context;
    // deleting copy constructor
    SingletonCharacteristics(const SingletonCharacteristics& obj) = delete;

    static SingletonCharacteristics* getInstance()
    {
        // If there is no instance of class
        // then we can create an instance.
        if (instancePtr == NULL)
        {
            instancePtr = new SingletonCharacteristics();

            return instancePtr;
        }
        else
        {
            return instancePtr;
        }
    }

    // sets values of member variables.
    void setChars(GattCharacteristic gattChar)
    {
        this->gattChars.push_back(gattChar);
    }

    // prints values of member variables
    std::vector<GattCharacteristic> getChars()
    {
        return this->gattChars;
    }
};


#endif