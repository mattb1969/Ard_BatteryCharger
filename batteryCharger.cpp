/*
 * Copyright (C) 2020 Matthew Bennett <matthew@mlbennett.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   batteryCharger.cpp
 * Author: matthew
 *
 * Created on 11 June 2020, 20:55
 */


#include        "batteryCharger.h"
#include        <Wire.h>

batteryCharger::batteryCharger()
//:
{
    return;
}

void batteryCharger::prepare() {
    Wire.begin();
    return;
}

bool batteryCharger::readPartNumber(int *part, char *description) {
    int status = false;
    int register_value = 0x00;
    int mask = 0b00111000;

    status = readRegister(byte(REG_VENDERPARTREVISIONSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 3) {
            case 4: //BQ24190:
                strcpy(description, "BQ24190");
                *part = BQ24190;
                status = true;
                break;
            case BQ24192:
                strcpy(description, "BQ24192");
                *part = BQ24192;
                status = true;
                break;
            case BQ24192I:
                strcpy(description, "BQ24192I");
                *part = BQ24192I;
                status = true;
                break;
            default:
                strcpy(description, "Unknown");
                *part = 0;
                status = false;
                break;
        }
    } else {
        strcpy(description, "unknown");
        *part = 0;
        status = false;
    }
    return status;

}

bool batteryCharger::readFaultRegister(int *fault) {
    // returns zero if all is well
    int status = false;
    int register_value = 0xFF;

    status = readRegister(byte(REG_FAULT), &register_value);

    if (status == true) {
        *fault = register_value;
    }

    return status;

}

bool batteryCharger::readFaultRegisterDetailed(int *fault, char *faultdescription) {
    /* BIT      FIELD TYPE      DESCRIPTION
     * Bit 7    WATCHDOG_FAULT  0 – Normal, 1- Watchdog timer expiration
     * Bit 6    BOOST_FAULT     0 – Normal, 1 – VBUS overloaded (OCP), or VBUS OVP in boost mode
     * Bit 5    CHRG_FAULT[1]   00 – Normal, 01 – Input fault (VBUS OVP or VBAT < VBUSS < 3.8 V), 
     * Bit 4    CHRG_FAULT[0]   10 - Thermal shutdown,11 – Charge Safety Timer Expiration
     * Bit 3    BAT_FAULT       0 – Normal, 1 – BATOVP
     * Bit 2    NTC_FAULT[2]    000 – Normal, 001 – TS1 Cold, 010 – TS1 Hot, 011 – TS2 Cold, 
     * Bit 1    NTC_FAULT[1]    100 – TS2 Hot, 101 – Both Cold, 110 – Both Hot
     * Bit 0    NTC_FAULT[0]
     * Max length of faultdescription is 133
     * Watchdog timer expiration; VBUS overloaded (OCP), or VBUS OVP in boost mode; Input fault (VBUS OVP or VBAT < VBUS < 3.8 V); Both Cold
     * 
     * returns zero if all is well*/


    int status = false;
    int register_value = 0xFF;
    int faultlogged = false;

    status = readRegister(byte(REG_FAULT), &register_value);

    if (status == true) {
        *fault = register_value;
        if ((register_value & 0x80) == 1) { //0b10000000
            strcpy(faultdescription, "Watchdog timer expired");
            faultlogged = true;
        };
        if ((register_value & 0x40) == 1) { //0b01000000
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "VBUS overloaded (OCP), or VBUS OVP in boost mode");
            faultlogged = true;
        };
        if ((register_value & 0x10) == 1) { //0b00-01-0000
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "Input fault (VBUS OVP or VBAT < VBUS < 3.8 V)");
            faultlogged = true;
        };
        if ((register_value & 0x20) == 1) { //0b00-10-0000
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "Thermal shutdown");
            faultlogged = true;
        };

        if ((register_value & 0x30) > 0) { //0b00-11-0000
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "Charge Safety Timer Expired");
            faultlogged = true;
        }
        if ((register_value & 0x08) == 1) { //0b00001000
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "BATOVP");
            faultlogged = true;
        }
        if ((register_value & 0x01) == 1) { //0b00000-001
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "TS1 Cold");
            faultlogged = true;
        }
        else if ((register_value & 0x02) == 1) { //0b00000-010
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "TS1 Hot");
            faultlogged = true;
        }
        else if ((register_value & 0x03) > 0) { //0b00000-011
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "TS2 Cold");
            faultlogged = true;
        }
        else if ((register_value & 0x04) == 1) { //0b00000-100
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "TS2 Hot");
            faultlogged = true;
        }
        else if ((register_value & 0x05) > 0) { //0b00000-101
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "Both Cold");
            faultlogged = true;
        }
        else if ((register_value & 0x06) > 0) { //0b00000-110
            if (faultlogged) strcpy(faultdescription, "; "); // previous fault 
            strcpy(faultdescription, "Both Hot");
            faultlogged = true;
        }
    }
    else {

        strcpy(faultdescription, "Error");
        *fault = 0xff;
        status = false;
    }
    return status;
}

bool batteryCharger::readVbusStatus(int *vbus, char *description) {
    int status = false;
    int register_value = 0x00;
    int mask = 0b11000000;

    /* Possible values
    00 – Unknown (no input, or DPDM detection incomplete), 01 – USB host, 10 – Adapter port, 11 – OTG */

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 6) {
            case 0x00:
                strcpy(description, "No Input");
                *vbus = 0;
                status = true;
                break;
            case 0x01:
                strcpy(description, "USB Host");
                *vbus = 1;
                status = true;
                break;
            case 0x02:
                strcpy(description, "Adaptor");
                *vbus = 2;
                status = true;
                break;
            case 0x03:
                strcpy(description, "On The Go");
                *vbus = 3;
                status = true;
                break;
            default:
                strcpy(description, "Error");
                *vbus = 0xff;
                status = false;
                break;
        }
    } else {

        strcpy(description, "Error");
        *vbus = 0xff;
        status = false;
    }
    return status;

}

bool batteryCharger::readDPMStatus(int *dpm, char *description) {
    //DPM - Dynamic Power Management
    int status = false;
    int register_value = 0x00;
    int mask = 0b00001000;

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 3) {
            case 0x00:
                strcpy(description, "Not DPM");
                *dpm = 0;
                status = true;
                break;
            case 0x01:
                strcpy(description, "Vin DPM");
                *dpm = 1;
                status = true;
                break;
            default:
                strcpy(description, "Error");
                *dpm = 0xff;
                status = false;
                break;
        }
    } else {

        strcpy(description, "Error");
        *dpm = 0xff;
        status = false;
    }
    return status;

}

bool batteryCharger::readPGStatus(int *pg, char *description) {
    //PG - Power Good Flag
    int status = false;
    int register_value = 0x00;
    int mask = 0b00000100;

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 2) {
            case 0x00:
                strcpy(description, "Not Power Good");
                *pg = 0;
                status = true;
                break;
            case 0x01:
                strcpy(description, "Power Good");
                *pg = 1;
                status = true;
                break;
            default:
                strcpy(description, "Error");
                *pg = 0xff;
                status = false;
                break;
        }
    } else {

        strcpy(description, "Error");
        *pg = 0xff;
        status = false;
    }
    return status;

}

bool batteryCharger::readThermalStatus(int *therm, char *description) {
    //PG - Power Good Flag
    int status = false;
    int register_value = 0x00;
    int mask = 0b00000010;

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 2) {
            case 0x00:
                strcpy(description, "Normal");
                *therm = 0;
                status = true;
                break;
            case 0x01:
                strcpy(description, "In Thermal Regulation");
                *therm = 1;
                status = true;
                break;
            default:
                strcpy(description, "Error");
                *therm = 0xff;
                status = false;
                break;
        }
    } else {

        strcpy(description, "Error");
        *therm = 0xff;
        status = false;
    }
    return status;

}

bool batteryCharger::readVsysminRegulation(int *vsys, char *description) {
    //PG - Power Good Flag
    int status = false;
    int register_value = 0x00;
    int mask = 0b00000001;

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 2) {
            case 0x00:
                strcpy(description, "Not in VSYSMIN regulation");
                *vsys = 0;
                status = true;
                break;
            case 0x01:
                strcpy(description, "In VSYSMIN regulation");
                *vsys = 1;
                status = true;
                break;
            default:
                strcpy(description, "Error");
                *vsys = 0xff;
                status = false;
                break;
        }
    } else {

        strcpy(description, "Error");
        *vsys = 0xff;
        status = false;
    }
    return status;

}

bool batteryCharger::readChargeStatus(int *charge, char *description) {
    int status = false;
    int register_value = 0x00;
    int mask = 0b00110000;

    /* Possible values
    00 – Not Charging, 01 – Pre-charge (<VBATLOWV), 10 – Fast Charging, 11 – Charge Termination Done */

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status) {
        switch ((register_value & mask) >> 4) {
            case 0x00:
                strcpy(description, "Not Charging");
                *charge = 0;
                status = true;
                break;
            case 0x01:
                strcpy(description, "Pre Charge");
                *charge = 1;
                status = true;
                break;
            case 0x02:
                strcpy(description, "Fast Charging");
                *charge = 2;
                status = true;
                break;
            case 0x03:
                strcpy(description, "Charge Done");
                *charge = 3;
                status = true;
                break;
            default:
                strcpy(description, "Error");
                *charge = 0xff;
                status = false;
                break;
        }
    } else {

        strcpy(description, "Error");
        *charge = 0xff;
        status = false;
    }
    return status;

}

bool batteryCharger::readRegister(byte regxx, int* contents) {
    int counter = 3;
    int errstatus = 0;
    int bytesreturned = 0;
    int bytessent = 0;
    int completed = false;
    if (DEBUG) {
        Serial.print("Reading Register: ");
        Serial.println(regxx);
    }

    do {
        Wire.beginTransmission((ADDRESS)); // >> 1)); // || READMODE);
        bytessent = Wire.write(regxx);
        if (DEBUG) {
            Serial.print("Begin Transmission, number of bytes sent: ");
            Serial.println(bytessent);
        }

        // Returns the number of bytes written, though reading that number is optional
        if (bytessent == 1) {
            errstatus = Wire.endTransmission(false);
            if (DEBUG) {
                Serial.print("end Transmission Error Status (0=good): ");
                Serial.println(errstatus);
            }
            if (errstatus == 0) {
                /*  0:success
                    1:data too long to fit in transmit buffer
                    2:received NACK on transmit of address
                    3:received NACK on transmit of data
                    4:other error                 */
                bytesreturned = Wire.requestFrom(ADDRESS, 1);
                // returns the number of bytes available.
                if (DEBUG) {
                    Serial.print("Request From number of bytes returned: ");
                    Serial.println(bytesreturned);
                }
                // the number of bytes returned from the slave device
                if (bytesreturned == 1) {
                    if (Wire.available() > 0) {
                        // The number of bytes available for reading.
                        *contents = Wire.read();
                        // The next byte received
                        if (Serial) {
                            Serial.print("Data Received: ");
                            Serial.println(*contents);
                        }
                        completed = true;
                    }
                }
            }
        }
        counter--;
    } while ((counter > 0) && (completed == false));
    return completed;
}

bool batteryCharger::overallStatus(int *sysstatus, int *faults) {
    // returns 2 bytes, system status and faults, only the latter can be used to get overall status
    int status = false;
    int register_value = 0xFF;

    status = readRegister(byte(REG_SYSTEMSTATUS), &register_value);

    if (status == true) {
        *sysstatus = register_value;
        register_value = 0xFF;
        status = readRegister(byte(REG_FAULT), &register_value);

        if (status == true) {
            *faults = register_value;
        }
    }
    
    return status;
}
//Overall status
// high byte of status
// low byte of faults
