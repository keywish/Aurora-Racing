/***********************************************************************
 *       __                                                          _
 *      / /        _____  __    __  _          _   (_)   ________   | |
 *     / /____   / _____) \ \  / / | |   __   | |  | |  (  ______)  | |_____
 *    / / ___/  | |_____   \ \/ /  | |  /  \  | |  | |  | |______   |  ___  |
 *   / /\ \     | |_____|   \  /   | | / /\ \ | |  | |  (_______ )  | |   | |
 *  / /  \ \__  | |_____    / /    | |/ /  \ \| |  | |   ______| |  | |   | |
 * /_/    \___\  \______)  /_/      \__/    \__/   |_|  (________)  |_|   |_|
 *
 *
 * Keywish Tech firmware
 *
 * Copyright (C) 2015-2020
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, in version 3.
 * learn more you can see <http://www.gnu.org/licenses/>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * [Title]
 * [Diagram]
 */
#include "AuroraRacing.h"
#include "ProtocolParser.h"
#include "KeyMap.h"
#include "Sounds.h"
#include "debug.h"

#define IR_PIN 8

#define BIN1_PIN 4
#define BIN2_PIN 2
#define PWMB_PIN 6
#define STANBY_PIN 7

#define SERVO_PIN 5
#define BUZZER_PIN 9
#define RGB_PIN A3

ProtocolParser *mProtocol = new ProtocolParser();
AuroraRacing mAuroraRacing(mProtocol, SERVO_PIN, BIN1_PIN, BIN2_PIN, PWMB_PIN, STANBY_PIN);
byte Ps2xStatus, Ps2xType;
ST_PROTOCOL SendData;

void setup() {
    Serial.begin(9600);
    mAuroraRacing.init();
    mAuroraRacing.SetServoBaseDegree(80);
    mAuroraRacing.SetControlMode(E_BLUETOOTH_CONTROL);
    mAuroraRacing.SetIrPin(IR_PIN);
    mAuroraRacing.SetBuzzerPin(BUZZER_PIN);
    mAuroraRacing.SetRgbPin(RGB_PIN);
    mAuroraRacing.Sing(S_connection);
    mAuroraRacing.SetDirection(90);
    mAuroraRacing.SetSpeed(0);
}

void HandleBloothRemote()
{
    if (mProtocol->ParserPackage()) {
        switch (mProtocol->GetRobotControlFun()) {
            case E_INFO:
                break;
            case E_ROBOT_CONTROL_DIRECTION:
                mAuroraRacing.SetDirection(mProtocol->GetRobotDegree());
                mAuroraRacing.Drive(mProtocol->GetRobotDegree());
                break;
            case E_ROBOT_CONTROL_SPEED:
                mAuroraRacing.SetSpeed(mProtocol->GetRobotSpeed());
                break;
            case E_CONTROL_MODE:
                mAuroraRacing.SetControlMode(mProtocol->GetControlMode());
                break;
            case E_BUZZER:
                // mAuroraRacing.Sing();
                // mAuroraRacing.PianoSing(mProtocol->GetPianoSing());
                break;
            case E_LED:
                mAuroraRacing.SetRgbLight(mProtocol->GetRgbValue());
                break;
            case E_VERSION:
                break;
        }
   }
}

void HandleInfaredRemote(byte irKeyCode)
{
    switch ((E_IR_KEYCODE)mAuroraRacing.mIrRecv->getIrKey(irKeyCode)) {
        case IR_KEYCODE_STAR:
             mAuroraRacing.SpeedUp(10);
             DEBUG_LOG(DEBUG_LEVEL_INFO, "mAuroraRacing.Speed = %d \n",mAuroraRacing.Speed);
            break;
        case IR_KEYCODE_POUND:
            DEBUG_LOG(DEBUG_LEVEL_INFO, " start Degree = %d \n", mAuroraRacing.Degree);
            mAuroraRacing.SpeedDown(10);
            break;
        case IR_KEYCODE_UP:
            mAuroraRacing.GoForward();
            break;
        case IR_KEYCODE_DOWN:
            mAuroraRacing.GoBack();
            break;
        case IR_KEYCODE_OK:
            mAuroraRacing.KeepStop();
            break;
        case IR_KEYCODE_LEFT:
            mAuroraRacing.TurnLeft(true);
            break;
        case IR_KEYCODE_RIGHT:
            mAuroraRacing.TurnRight(true);
            break;
        default:
            break;
    }
  //  mAuroraRacing.mIrRecv->resume();
}

void HandleInfraredTracing(void)
{
    switch (mAuroraRacing.mInfraredTracing->getValue()) {
     case IT_ALL_BLACK:
        mAuroraRacing.KeepStop();
        break;
     case IT_ALL_WHITE:
        mAuroraRacing.KeepStop();
        break;
     case IT_CENTER:
        mAuroraRacing.SetSpeed(35);
        mAuroraRacing.GoForward();
        break;
     case IT_RIGHT1:
        mAuroraRacing.SetDirection(70);
        mAuroraRacing.Drive();
        mAuroraRacing.SetSpeed(25);
        break;
     case IT_RIGHT2:
        mAuroraRacing.SetDirection(50);
        mAuroraRacing.Drive();
        mAuroraRacing.SetSpeed(24);
        break;
     case IT_RIGHT3:
        mAuroraRacing.SetDirection(40);
        mAuroraRacing.Drive();
        mAuroraRacing.SetSpeed(22);
       delay(450);
        break;
     case IT_LEFT1:
        mAuroraRacing.SetDirection(170);
        mAuroraRacing.Drive();
        mAuroraRacing.SetSpeed(22);
        delay(460);
        break;
     case IT_LEFT2:
        mAuroraRacing.SetDirection(130);
        mAuroraRacing.Drive();
        mAuroraRacing.SetSpeed(25);
        break;
    case IT_LEFT3:
        mAuroraRacing.SetDirection(110);
        mAuroraRacing.Drive();
        mAuroraRacing.SetSpeed(24);
        break;
   }
}

void HandlePs2xRemote()
{
    static int vibrate = 0;
    byte PSS_X = 0, PSS_Y = 0;
    mAuroraRacing.mPs2x->read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed
    if (mAuroraRacing.mPs2x->ButtonDataByte()) {
        if (mAuroraRacing.mPs2x->Button(PSB_PAD_UP)) {     //will be TRUE as long as button is pressed
            mAuroraRacing.GoForward();
        }
        if (mAuroraRacing.mPs2x->Button(PSB_PAD_RIGHT)) {
            mAuroraRacing.TurnRight(true);
        }
        if (mAuroraRacing.mPs2x->Button(PSB_PAD_LEFT)) {
            mAuroraRacing.TurnLeft(true);
        }
        if (mAuroraRacing.mPs2x->Button(PSB_PAD_DOWN)) {
            mAuroraRacing.GoBack();
        }

        vibrate = mAuroraRacing.mPs2x->Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
        if (mAuroraRacing.mPs2x->Button(PSB_CROSS)) {             //will be TRUE if button was JUST pressed OR released
            mAuroraRacing.SpeedDown(5);
        }
        if (mAuroraRacing.mPs2x->Button(PSB_TRIANGLE)) {
            mAuroraRacing.SpeedUp(5);
        }
        if (mAuroraRacing.mPs2x->Button(PSB_CIRCLE)) {
            mAuroraRacing.TurnRight(false);
        }
        if (mAuroraRacing.mPs2x->Button(PSB_SQUARE)) {
            mAuroraRacing.TurnLeft(false);
        }
        if (mAuroraRacing.mPs2x->ButtonReleased(PSB_CIRCLE) || mAuroraRacing.mPs2x->ButtonReleased(PSB_SQUARE)) {
           mAuroraRacing.SetDirection(90);
        }
        if (mAuroraRacing.mPs2x->Button(PSB_L1) || mAuroraRacing.mPs2x->Button(PSB_R1)) {
            PSS_X = mAuroraRacing.mPs2x->Analog(PSS_LY);
            PSS_Y = mAuroraRacing.mPs2x->Analog(PSS_LX);
            DEBUG_LOG(DEBUG_LEVEL_INFO, "%x %x ", PSS_X, PSS_Y);
            mAuroraRacing.mRgb->setColor(E_RGB_LEFT, PSS_X, PSS_Y, PSS_X + PSS_Y);
            PSS_X = mAuroraRacing.mPs2x->Analog(PSS_RY);
            PSS_Y = mAuroraRacing.mPs2x->Analog(PSS_RX);
            DEBUG_LOG(DEBUG_LEVEL_INFO, "%x %x\n", PSS_X, PSS_Y);
            mAuroraRacing.mRgb->setColor(E_RGB_RIGHT, PSS_X, PSS_Y, PSS_X + PSS_Y);
            mAuroraRacing.mRgb->show();
        }
    } else {
        mAuroraRacing.KeepStop();
    }
    delay(50);
}

void SendTracingSignal(){
    byte TracingSignal = mAuroraRacing.mInfraredTracing->getValue();
    SendData.start_code = 0xAA;
    SendData.type = 0x01;
    SendData.addr = 0x01;
    SendData.function = E_INFRARED_TRACKING;
    SendData.data = (byte *)&TracingSignal;
    SendData.len = 7;
    SendData.end_code = 0x55;
    mProtocol->SendPackage(&SendData, 1);
}

void loop() {
    static byte mode;
    mProtocol->RecevData();
    if (mAuroraRacing.GetControlMode() !=  E_BLUETOOTH_CONTROL &&  mAuroraRacing.GetControlMode() != E_PIANO_MODE) {
        if (mProtocol->ParserPackage()) {
            if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
            mAuroraRacing.SetControlMode(mProtocol->GetControlMode());
           }
        }
    }
    switch (mAuroraRacing.GetControlMode()) {
        case E_BLUETOOTH_CONTROL:
            HandleBloothRemote();
           // DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUTOOTH_CONTROL \n");
            break;
        case E_INFRARED_REMOTE_CONTROL:
            byte irKeyCode;
            if (irKeyCode = mAuroraRacing.mIrRecv->getCode()) {
                DEBUG_LOG(DEBUG_LEVEL_INFO, "irKeyCode = %lx \n", irKeyCode);
                HandleInfaredRemote(irKeyCode);
                delay(110);
            } else {
                if (mAuroraRacing.GetStatus() != E_STOP ) {
                    mAuroraRacing.KeepStop();
                }
            }
            break;
        case E_INFRARED_TRACKING_MODE:
            // DEBUG_LOG(DEBUG_LEVEL_INFO, "E_INFRARED_TRACKING \n");
            if(mode != E_INFRARED_TRACKING_MODE) {
                mAuroraRacing.SetInfraredTracingPin(AR_INFRARED_TRACING_PIN1, AR_INFRARED_TRACING_PIN2, AR_INFRARED_TRACING_PIN3, AR_INFRARED_TRACING_PIN4, AR_INFRARED_TRACING_PIN5);
                delay(200);
                mode = E_INFRARED_TRACKING_MODE;
            }
            mAuroraRacing.SetInfraredTracingPin(AR_INFRARED_TRACING_PIN1, AR_INFRARED_TRACING_PIN2, AR_INFRARED_TRACING_PIN3, AR_INFRARED_TRACING_PIN4, AR_INFRARED_TRACING_PIN5);
            HandleInfraredTracing();
            SendTracingSignal();
            break;
        case E_PS2_REMOTE_CONTROL:
            if(mode != E_PS2_REMOTE_CONTROL) {
              Ps2xStatus = mAuroraRacing.SetPs2xPin(AR_PS2X_CLK, AR_PS2X_CMD, AR_PS2X_CS, AR_PS2X_DAT);
              Ps2xType = mAuroraRacing.mPs2x->readType();
              delay(500);
              mode = E_PS2_REMOTE_CONTROL;
            }
            while (Ps2xStatus != 0) { //skip loop if no controller found
                delay(500);
                Ps2xStatus = mAuroraRacing.ResetPs2xPin();
                Ps2xType = mAuroraRacing.mPs2x->readType();
                DEBUG_LOG(DEBUG_LEVEL_INFO, "ps2x reconnect status = %d, type = %d\n", Ps2xStatus, Ps2xType);
            }
            if (Ps2xType != 2) {
                // Guitar Hero Controller
                HandlePs2xRemote();
            }
            break;
         case E_PIANO_MODE:
            if (mProtocol->ParserPackage()) {
                if (mProtocol->GetRobotControlFun() == E_BUZZER) {
                    mAuroraRacing.PianoSing(mProtocol->GetPianoSing());
                } else if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
                    mAuroraRacing.SetControlMode(mProtocol->GetControlMode());
                }
            }
            break;
        default:
            break;
    }
    switch (mAuroraRacing.GetStatus()) {
        case E_FORWARD:
             mAuroraRacing.LightOn();
            break;
        case E_LEFT:
            mAuroraRacing.LightOn(E_RGB_LEFT);
            break;
        case E_RIGHT:
            mAuroraRacing.LightOn(E_RGB_RIGHT);
         //   mAuroraRacing.Sing(S_OhOoh);
            break;
        case E_BACK:
            mAuroraRacing.LightOn(E_RGB_ALL, RGB_RED);
            break;
        case E_STOP:
            mAuroraRacing.LightOff();
            break;
        case E_SPEED_UP:
            mAuroraRacing.mBuzzer->_tone(5000, 50, 20);
            mAuroraRacing.LightOn(E_RGB_ALL, mAuroraRacing.GetSpeed()*2.5);
            break;
        case E_SPEED_DOWN:
            mAuroraRacing.mBuzzer->_tone(1000, 50, 20);
            mAuroraRacing.LightOn(E_RGB_ALL, mAuroraRacing.GetSpeed()*2.5);
            break;
        default:
            break;
    }
}
