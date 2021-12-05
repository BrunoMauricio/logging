# Logging

A basic logging mechanism for centralized log handling

## Concept
Logging is one of the most important mechanisms in any system.

As such, it is preferable that it can be easily done and globally configurable.

This logging mechanism makes use of C++ stream concept to provide broad output capabilities.

There are several different logging levels, with different intents priorities and output formats:

The logging levels are based on the SNMP ones:

Official SNMP meanings

    Name            Description

Debug:         Debug messages

Informational: Informational messages

Notice:        Normal but significant conditions exist

Warning:       Warning conditions exist

Error:         Error conditions exist. Perror is displayed

Critical:      Critical conditions exist

Alert:         Immediate action needed

Emergency:     System unusable

This projects' meanings

    Name            Description

Debug:         Messages regarding the state of the code. Should be disabled normally (elevated logging load)

               Ex: Sending message to peer X on socket Y. *Message Dump*

Informational: Messages regarding the state of the system in some detail (medium logging load)

               Ex: New socket Y opened with peer X!

Notice:        Relevant system state

               Ex: Received chat message from peer X.

Warning:       There are problematic conditions or inconsistencies

               Ex: Socket Ys' buffer is too full

Error:         System errors. Prints errno error code and respective string.

               Ex: Socket Y errored out unexpectedly on send/receive

Critical:      Critical conditions exist. Some system modules/features may be down.

               Ex: Cannot open base server socket

Alert:         Immediate action needed. System partially down and may require restart/fix

               Ex: Database is corrupted, cannot open main menu.

Emergency:     System unusable and/or down

               Ex: The number 2 ceased to exist. Help.

## Usage


## Roadmap
* Main priority
* * Add configurable flushing strategies for the different log levels
* * Add a way to disable resource consuming levels like debug
* Secondary priority
* * Maybe add colors?

## Contributing
This is a proof of concept, and perhaps even useless, but I'm open to suggestions.

## Authors and acknowledgment
The base for this code was taken from https://newbedev.com/overload-handling-of-std-endl, and adapted by Bruno Mauricio (@BrunoMauricio)

## License
This project is not meant/allowed to be used anywhere. This may change in the future.

## Project status
On going


