/* 
 VGA Output Viewer
 Created by Nusaiba Rakhshan for ECE241 Fall 2023

 Purpose: Convert a modelsim log into a ppm to view what a single frame of VGA output will look like

 How to use:
 1. Log the VGA signals using modelsim. The following code will generate or clear the file log.txt
 then log the signals writeEn, x, y and colour every positive clock edge. Colour is split into 3 8-bit 
 values representing r, g and b. Maintain the spaces between each signal, but change the names in the square 
 braces to whatever your signal names are. Change "clock" as well if applicable. Insert this in your 
 do file and it will log all the signals run AFTER this snippet. 

  -code snippet begin-

  set fp [open log.txt w]
  when {clock} {
    set fp [open log.txt a]
    puts $fp "[examine writeEn] [examine x] [examine y] [examine {colour[23:16]}] [examine {colour[15:8]}] [examine {colour[7:0]}]"
    close $fp
  }

  -code snippet end-
 2. See the comments throughout the code below and change any values applicable.
 3. Compile and run. Ignore the warnings LOL
 4. Use a program or website to view the ppm file generated. I used the website https://0xc0de.fr/webppm/
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int binaryToDecimal(int n);

int main () {
  int BG = 0;
  int XRES = 160; //Change to the resolution of your VGA Output
  int YRES = 120;
  int ena[XRES][YRES];
  int red[XRES][YRES];
  int green[XRES][YRES];
  int blue[XRES][YRES];

  for (int j = 0; j < YRES; j++){
    for (int i = 0; i < XRES; i++){
      ena[i][j] = 0;
      red[i][j] = BG;
      green[i][j] = BG;
      blue[i][j] = BG;
    }
  }

  string line;
  ifstream myfile ("../v5_bg/log.txt"); //Change to the absolute or relative path of your modelsim log file
  if (myfile.is_open()){
    while (getline (myfile, line)){
      stringstream lineStream (line);
      string enable;
      int x;
      int y;
      int r;
      int g;
      int b;
      lineStream >> enable;
      lineStream >> x;
      lineStream >> y;
      lineStream >> r;
      lineStream >> g;
      lineStream >> b;

      x = binaryToDecimal(x);
      y = binaryToDecimal(y);
      r = binaryToDecimal(r);
      g = binaryToDecimal(g);
      b = binaryToDecimal(b);

      if (enable == "St0"){
        ena[x][y] = 0; 
      }
      else if (x < XRES && y < YRES){
        ena[x][y] = 1; 
        red[x][y] = r;
        green[x][y] = g;
        blue[x][y] = b;
      }
    }
    myfile.close();
  }

  ofstream write;
  write.open ("vga.ppm"); //Change to the name you want your generated ppm file to have. 
                          //It will generate in the same directory that this cpp file is in unless you specify a different path
  write << "P3 " << XRES << " " << YRES << " 255" << endl;
  for (int j = 0; j < YRES; j++){
    for (int i = 0; i < XRES; i++){
      write << red[i][j] << " " << green[i][j] << " " << blue[i][j] << endl; 
    }
  }
  write.close();

  return 0;
}

int binaryToDecimal(int n){
    int value = 0;
    int base = 1;

    int temp = n;
    while (temp != 0) {
      int digit = temp % 10;
      temp = temp / 10;

      value += digit * base;

      base = base * 2;
    }

    return value;
}