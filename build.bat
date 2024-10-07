@echo off
clang++ ./subnetMask/*.h ./subnetMask/*.cpp ./Gateway/*.h ./Gateway/*.cpp *.cpp -std=c++20 -Os