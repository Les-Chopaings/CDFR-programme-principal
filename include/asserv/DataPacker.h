#pragma once
#ifndef PLATFORMIO
#include <iostream>
#include <vector>
#endif
#include <cstdint>

#ifdef PLATFORMIO
    class DataPacker {
    private:
        uint8_t* data;
        int lenght;
        int currentIndex;

    public:
        DataPacker(uint8_t* array, int lengthArray) : data(array), lenght(lengthArray), currentIndex(0) {}

        void addUint8(uint8_t value) {
            data[currentIndex] = (uint8_t)(value);
            currentIndex++;
        }

        void addUint16(uint16_t value) {
            data[currentIndex]     = (uint8_t)((value >> 8) & 0xFF);
            data[currentIndex + 1] = (uint8_t)(value & 0xFF);
            currentIndex += 2;
        }

        void addArray(const uint8_t* array, int length) {
            for(int i = 0; i < length; i++){
                addUint8(array[i]);
            }
        }

        uint8_t* getData() {
            return data;
        }

        int getSize() const {
            return currentIndex;
        }

        // void printData() const {
        //     for (auto val : data) {
        //         std::cout << static_cast<int>(val) << " ";
        //     }
        //     std::cout << std::endl;
        // }
    };

#else
    class DataPacker {
    private:
        std::vector<uint8_t> data;

    public:
        DataPacker(){}

        void addUint8(uint8_t value) {
            data.push_back(value);
        }

        void addUint16(uint16_t value) {
            data.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
            data.push_back(static_cast<uint8_t>(value & 0xFF));
        }

        void addArray(const uint8_t* array, size_t length) {
            data.insert(data.end(), array, array + length);
        }

        uint8_t* getData() {
            return data.data();
        }

        size_t getSize() const {
            return data.size();
        }

        void printData() const {
            for (auto val : data) {
                std::cout << static_cast<int>(val) << " ";
            }
            std::cout << std::endl;
        }
    };
#endif

