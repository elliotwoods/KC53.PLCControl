# Information

* The "end stop" is the sensor that detect when the carriage is near the end of motion at either end
* The "stopper" is the physical hard stop that stops the movement mechanically. We want to avoid hitting the hard stop.
* Installation "Right" = Inverter "Reverse"
* Installation "Left" = Inverter "Forward"

# Notes for setup

* Setup the end stops so that they are active whilst the carriage reaches the stopper, and as early as possible before that (otherwise the carriage can be stuck on the wrong side of the end stop without the control program being aware).
* The carriage will begin deccelerating when it meets the stopper. Please tune the decceleration parameter so that the carriage eventually stops a safe distance (e.g. 50mm) before the it reaches the stopper.
