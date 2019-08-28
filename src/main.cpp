// Copyright 2019 FURGBot

#include "serial_sender.h"
#include "manual_control.h"
#include "parameters.h"

int main() {
    furgbol::parameters::Parameters param;

    furgbol::joystick::SerialSender serial_sender(param.serial_port);

    ManualControl joystick(0, param, &serial_sender);

    joystick.start();

    while(1);
}