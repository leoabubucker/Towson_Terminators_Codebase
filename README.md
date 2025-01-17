![Static Badge](https://img.shields.io/badge/VEX_VRC-blue)  ![Static Badge](https://img.shields.io/badge/Towson_Robotics-%23800000)

# Towson Terminators Codebase
 - C++ robotics code that was created by the Towson High School Robotics Organization from the years 2021-Present.
 - A Python GUI application for attendance logging. 

## Usage 
This codebase has been licensed with an MIT license to allow and promote free and open access to code that is necessary or useful for VEX VRC teams. 
This codebase has been created primarily for the following reasons:
1. To allow our documenters to readily incorporate code into engineering notebooks.
2. To serve as guidance for future programmers on our team and maybe beyond.
3. To serve as a historical record of our team's C++ programming.

As per the MIT License, all/any part(s) of this codebase may be forked and modified for use. Please read the notes on directory structure and 2021-2022 code below for guidance. We hope this codebase helps guide you on your programming journey! 
#### ATTENTION VEX VRC COMPETITIVE TEAMS - Even though this code is licensed with the MIT license, the RECF Code of Conduct and VEX Game Manual rules, particularly, G4 (The Robot must represent the skill level of the team) and R2 (Robots must represent the Team's skill level) may prohibit direct copy/paste of code in this repository for competitive use. Teams are encouraged to use this repository as guidance but are reminded to fully understand and be able to explain all code to event staff at request. Towson Robotics DOES NOT condone actions that violate either the letter or spirit of the RECF Code of Conduct and/or given VEX Game Manual. 

## Directory Structure
 - The "Attendance_Tracker" folder contains a standalone Python application that is used to track attendance for meetings. More details can be found in the "AttendanceTracker_README.md" file in the folder. 
 - The "Competitive_Code" folder contains all code actually used at VEX VRC competitions by the Towson Terminators, sorted by season.
 - The "Non-Competitve_Code" folder contains all code that was either never intended for or ultimately never used in actual competitions, sorted by season. 
 - All C++ header (.h) files can be found in any folders titled "include"
 - All C++ source (.cpp) files can be found in any folders titled "src"
 - C++ Code execution always starts in the main.cpp file. 

## A Note About PROS Code
PROS code is any folder that contains a "project.pros" file in it. PROS code was first used in this repository in the 2024-2025 season. PROS code uses the PROS API developed by Purdue University instead of the VEX API developed by VEX Robotics. More information on the PROS API can be found on their [website](https://pros.cs.purdue.edu/). PROS code requires the PROS extension in VSCode which can be set up as per instructions [here](https://pros.cs.purdue.edu/v5/getting-started/). The VEX API and PROS API are completely different APIs - code is not equivalent and one cannot be run with the other's extension. 

## A Note About 2021-2022 Code
2021-2022 code was written in the now deprecated VEXcode Pro V5 application and can be either read in the app (if still available) or imported via the GUI available in the VEX Robotics extension in VScode. Download link for the VEXcode Pro V5 application (if still available): [VEXcode Pro V5](https://www.vexrobotics.com/vexcode/install/v5). Make sure to get the "VEXcode Pro V5" not the "VEXcode V5 (Blocks and Text)."

## Contributors
- Leo Abubucker (leoabubucker): Codebase Creator and Programmer 2021-2025
- Apia To'oto'o (ZapacTheGreat): Codespaces Tester 2024
- Jack Deise (Jukester123): Programmer-In-Training 2024-2025
- Daksh Sangar (Daksh-Sangar): 934A Programmer 2024-2025
  
## Software License
[MIT](https://choosealicense.com/licenses/mit/)

Copyright (c) 2024 Leo Abubucker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
