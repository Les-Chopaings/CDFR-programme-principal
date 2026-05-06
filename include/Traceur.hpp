#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>

class Traceur {
public:
    int width;
    int height;
    int mapScale;
    std::vector<std::vector<uint8_t>> data;

    Traceur(int w, int h, int s) : mapScale(s){
        width = w/s;
        height = h/s;
        data.resize(height, std::vector<uint8_t>(width, 0));
    }

    // Vérifie si un point est dans la matrice
    bool inBounds(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    // Trace un point
    void drawPoint(int x, int y, uint8_t val) {
        x = x/mapScale;
        y = y/mapScale;

        if (inBounds(x, y)) {
            data[y][x] = val;
        }
    }

    // Trace une ligne avec l'algo de Bresenham
    void drawLine(int x1, int y1, int x2, int y2, uint8_t val) {
        x1 = x1/mapScale;
        y1 = y1/mapScale;
        x2 = x2/mapScale;
        y2 = y2/mapScale;

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);

        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;

        int err = dx - dy;

        while (true) {
            if (inBounds(x1, y1)) {
                data[y1][x1] = val;
            }

            if (x1 == x2 && y1 == y2)
                break;

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

    std::string getString() {
        std::ostringstream oss;
        for (int y = height-1 ; y >=0 ; y--) {
            for (int x = 0; x < width; x++) {
                switch (data[y][x])
                {
                case 0:
                    oss << "..";
                    break;
                case 1:
                    oss << "##";
                    break;

                case 2:
                    oss << "##";
                    break;

                case 3:
                     oss << "##";
                    break;

                case 4:
                     oss << "SS";
                    break;

                case 5:
                     oss << "EE";
                    break;

                default:
                    oss << "..";
                    break;
                }
            }
            oss << "\n";
        }
        return oss.str();
    }

    // Affichage simple (debug)
    void print() {
        for (int y = height-1 ; y >=0 ; y--) {
            for (int x = 0; x < width; x++) {
                switch (data[y][x])
                {
                case 0:
                    std::cout << "..";
                    break;
                case 1:
                    std::cout << "\033[31m"<<"##"<<"\033[0m";
                    break;

                case 2:
                    std::cout << "##";
                    break;

                case 3:
                     std::cout << "\033[32m"<<"##"<<"\033[0m";
                    break;

                case 4:
                     std::cout << "\033[32m"<<"SS"<<"\033[0m";
                    break;

                case 5:
                     std::cout << "\033[32m"<<"EE"<<"\033[0m";
                    break;

                default:
                    std::cout << "..";
                    break;
                }
            }
            std::cout << "\n";
        }
    }
};