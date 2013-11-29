/*
for uno sd card should connect to 11 (MOSI), 12 (MISO), 13 (SCK)
 for 2560 it should be 51 (MOSI), 50 (MISO), 52 (SCK)
 */
#include <SD.h>
#include <SPI.h>
//set vs1003 pins
int dreq= 6;
int xCs=7;
int xDcs=8;
int xReset=9;

//for sd test
File root;
int i=0;
int fileNumber=0;

int sspin=53; //SD Select pin

int volume=0x30;//set volume here 

//play control
int DREQ;
File mp3;

void setup(){
  pinMode(dreq,INPUT);
  pinMode(xCs,OUTPUT);
  pinMode(xDcs,OUTPUT);
  pinMode(xReset,OUTPUT);
  pinMode(sspin,OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST); //send most-significant bit first
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2); 
  digitalWrite(sspin,HIGH);
  Mp3Reset();
  digitalWrite(sspin,LOW);
  
  Serial.begin(9600);
  Serial.print("Set SD card..."); 
  while (!SD.begin()){   
  }
  Serial.println("sd ready."); 
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.print("Totally ");
  Serial.print(fileNumber);
  Serial.print(" file(s)");
  Serial.println();
}

void loop(){
  play("song1.mp3");
}

void play(char* playplay){
  int val,i;
  mp3=SD.open(playplay);
  delay(10);
  for(i=0;i<548;i++){
    digitalWrite(sspin,HIGH);
    val =mp3.read();
    digitalWrite(xDcs,LOW);
    SPI.transfer(val);
    digitalWrite(xDcs,HIGH);
    digitalWrite(sspin,LOW);
  }
  //if(DREQ==HIGH){
    while(mp3.available()){
      for(i=0;i<32;i++){
        digitalWrite(sspin,HIGH);
        val =mp3.read();
        digitalWrite(xDcs,LOW);
        SPI.transfer(val);
        digitalWrite(xDcs,HIGH);
        digitalWrite(sspin,LOW);
      }
      delayMicroseconds(35);
    }
  //}
  mp3.close();
}

void Mp3Reset(){
  digitalWrite(xReset,LOW);
  digitalWrite(xCs,HIGH);
  digitalWrite(xDcs,HIGH);
  delay(100);
  digitalWrite(xReset,HIGH);
  sciWrite(0x00,0x08,0x00); //向MODE中写入
  sciWrite(0x03,0xF8,0x00);//设置VS1003的时钟
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  sciWrite(0X02,0X00,0X00);
  sciWrite(0X0B,volume,volume);//音量最高0x0000音量最低0xFEFE
  while(!digitalRead(dreq)){
  }
  digitalWrite(xReset,HIGH);
  digitalWrite(xCs,HIGH);
  digitalWrite(xDcs,HIGH);
}

void sciWrite(unsigned char addr,unsigned char hdat,unsigned char ldat)
{  
  while(!digitalRead(dreq)){
  }
  if(digitalRead(dreq)){  //double check
    digitalWrite(xCs,LOW);
    SPI.transfer(0x02);
    SPI.transfer(addr);
    SPI.transfer(hdat);
    SPI.transfer(ldat);    
    digitalWrite(xCs,HIGH);
  }
}

//void sciRead(unsigned char addr,

void printDirectory(File dir, int numTabs) {
  while(i<6){
    File entry =  dir.openNextFile();
    if (! entry) { // no more files
      i++;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()){
      Serial.println("/");
      printDirectory(entry, numTabs+1);
    }
    else if(entry.size()) {// files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
      fileNumber++;
    }
  }
}
