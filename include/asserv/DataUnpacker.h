#pragma once

#ifndef PLATFORMIO
#include <iostream>
#include <vector>
#include <stdexcept>
#endif
#include <cstdint>

#ifdef PLATFORMIO
    class DataUnpacker {
    private:
        const uint8_t* data;
        int lenght;
        int currentIndex;

    public:
        DataUnpacker(const uint8_t* array, int lengthArray) : data(array), lenght(lengthArray), currentIndex(0) {}

        uint8_t popUint8() {
            return data[currentIndex++];
        }

        uint16_t popUint16() {
            uint16_t value = (uint16_t)(data[currentIndex + 1]) |
                            ((uint16_t)(data[currentIndex]) << 8);
            currentIndex += 2;
            return value;
        }

        // void printData() const {
        //     for (size_t i = currentIndex; i < data.size(); ++i) {
        //         std::cout << static_cast<int>(data[i]) << " ";
        //     }
        //     std::cout << std::endl;
        // }
    };

#else
    class DataUnpacker {
    private:
        std::vector<uint8_t> data;
        size_t currentIndex;

    public:
        DataUnpacker(const uint8_t* array, size_t length) : data(array, array + length), currentIndex(0) {}

        uint8_t popUint8() {
            if (currentIndex >= data.size()) {
                throw std::out_of_range("No more data to pop");
            }
            return data[currentIndex++];
        }

        uint16_t popUint16() {
            if (currentIndex + 1 >= data.size()) {
                throw std::out_of_range("No more data to pop");
            }
            uint16_t value = static_cast<uint16_t>(data[currentIndex + 1]) |
                            (static_cast<uint16_t>(data[currentIndex]) << 8);
            currentIndex += 2;
            return value;
        }

        void printData() const {
            for (size_t i = currentIndex; i < data.size(); ++i) {
                std::cout << static_cast<int>(data[i]) << " ";
            }
            std::cout << std::endl;
        }
    };
#endif
