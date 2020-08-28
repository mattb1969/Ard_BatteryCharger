/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.ino
 * Author: matthew
 *
 * Created on 06 May 2020, 18:07
 *
 * Overview: Example Code to communicate with the Battery Charger chip on the mkr wan 1310
 */


#include    <MKRWAN.h>

#include    "batteryCharger.h"

batteryCharger      bc;

void setup() {
    
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Battery Charger Example");
    delay(500);

    bc.prepare();
}

void loop() {

    int     partno = 0;
    char    description[] = ".............";
    int     fault = 0;
    char    faultdescription[133] = ".";
    int     vbus = 0xff;
    int     dpm = 0xff;
    int     pg = 0xff;
    int     therm = 0xff;
    int     vsys = 0xff;
    int     charge = 0xff;
    int     sysstatus = 0xff, faults = 0xff;

    Serial.println("Reading Overall Status");
    Serial.println("======================");
    if (bc.overallStatus(&sysstatus, &faults)) {
        Serial.print("Overall Status: ");
        Serial.print(sysstatus);
        Serial.print(" Fault Status: ");
        Serial.println(faults);
    }
    else {
        Serial.println("Failed to read overall status!");
        delay(2000);
    }            
            
    Serial.println("Reading Part Number");
    Serial.println("===================");
    if (bc.readPartNumber(&partno, description)) {
        Serial.print("Model Number: ");
        Serial.print(partno);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read part number!");
        delay(2000);
    }

    Serial.println();
    Serial.println("Reading Fault Register");
    Serial.println("======================");
    if (bc.readFaultRegister(&fault)) {
        Serial.print("Fault Status: ");
        Serial.println(fault);
    }
    else {
        Serial.println("Failed to read fault status!");
        delay(2000);
    }
    
    Serial.println();
    Serial.println("Reading Fault Register Details");
    Serial.println("==============================");
    if (bc.readFaultRegisterDetailed(&fault, faultdescription)) {
        Serial.print("Fault Status: ");
        Serial.println(fault);
        Serial.println("Fault Description:");
        Serial.println(faultdescription);
    }
    else {
        Serial.println("Failed to read detailed fault information!");
        delay(2000);
    }
        
    Serial.println();
    Serial.println("Reading Vbus Status");
    Serial.println("===================");
    if (bc.readVbusStatus(&vbus, description)) {
        Serial.print("Vbus Status: ");
        Serial.print(vbus);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read VBus status!");
        delay(2000);
    }
        
    Serial.println();
    Serial.println("Reading Charge Status");
    Serial.println("=====================");
    if (bc.readChargeStatus(&charge, description)) {
        Serial.print("Charge Status: ");
        Serial.print(charge);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read Charge status!");
        delay(2000);
    }
        
    Serial.println();
    Serial.println("Reading DPM Status");
    Serial.println("==================");
    if (bc.readDPMStatus(&dpm, description)) {
        Serial.print("DPM Status: ");
        Serial.print(dpm);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read DPM status!");
        delay(2000);
    }
     
    Serial.println();
    Serial.println("Reading PG Status");
    Serial.println("==================");
    if (bc.readPGStatus(&pg, description)) {
        Serial.print("PG Status: ");
        Serial.print(pg);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read PG status!");
        delay(2000);
    }
    
    Serial.println();
    Serial.println("Reading Thermal Status");
    Serial.println("=====================");
    if (bc.readThermalStatus(&therm, description)) {
        Serial.print("Thermal Status: ");
        Serial.print(therm);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read Thermal status!");
        delay(2000);
    }

    Serial.println();
    Serial.println("Reading Vsys Regulation");
    Serial.println("=======================");
    if (bc.readVsysminRegulation(&vsys, description)) {
        Serial.print("Vsys In Regulation: ");
        Serial.print(vsys);
        Serial.print(" ");
        Serial.println(description);
    }
    else {
        Serial.println("Failed to read vsys regulation!");
        delay(2000);
    }            
    delay(500);
}

