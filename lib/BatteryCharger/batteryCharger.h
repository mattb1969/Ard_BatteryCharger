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
 * File:   batteryCharger.h
 * Author: matthew
 *
 * Created on 11 June 2020, 20:55
 */

#ifndef BATTERYCHARGER_H
#define BATTERYCHARGER_H

#include    <Arduino.h>

#define         ADDRESS             0x6B
#define         READMODE            0x00                // 0b00000000
#define         WRITEMODE           0x01                // 0b00000001

#define         BQ24190             0x04                // 0x23 - 0bxx100xxx
#define         BQ24192             0x05                // 0x2B - 0bxx101xxx
#define         BQ24192I            0x01                // 0x0b - 0bxx001xxx

#define         REG_INPUTSOURCE                                 0x00
#define         REG_POWERONCONFIGURATION                        0x01
#define         REG_CHARGECURRENT                               0x02
#define         REG_PRECHARGETERMINATIONCURRENT                 0x03
#define         REG_CHARGEVOLTAGECONTROL                        0x04
#define         REG_CHARGETERMINATIONTIMERCONTROL               0x05
#define         REG_IRCOMPENSATIONTHERMALREGULATIONCONTROL      0x06
#define         REG_MISCOPERATIONCONTROL                        0x07
#define         REG_SYSTEMSTATUS                                0x08
#define         REG_FAULT                                       0x09
#define         REG_VENDERPARTREVISIONSTATUS                    0x0A

#define         DEBUG               false


/* Return values
   function response is a bool - status of the command, true = successful
 */
class batteryCharger {
    public:
        batteryCharger();
        void prepare();
        bool readPartNumber(int *part, char *description);
        bool readFaultRegister(int *fault);
        bool readFaultRegisterDetailed(int *fault, char *faultdescription);
            // Note: Fault Description must be a minimum of 140 characters long.
        bool readVbusStatus(int *vbus, char *description);
        bool readDPMStatus(int *dpm, char *description);
        bool readPGStatus(int *pg, char *description);
        bool readThermalStatus(int *therm, char *description);
        bool readVsysminRegulation(int *vsys, char *description);
        bool readChargeStatus(int *charge, char *description);
        bool overallStatus(int *sysstatus, int *faults);



    private:
        bool readRegister(byte regxx, int* contents);


};


#endif /* BATTERYCHARGER_H */

