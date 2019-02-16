# MQTT to Mendeleev Bridge

This is a bridge to control the Mendeleev project over MQTT. More info on the Mendeleev protocol [here](https://github.com/area3001/mendeleev).

## Basic Usage

> Address is the address of the node to query. It corresponds to the atomic number.

**MQTT publish commands:**

    echo -en "\xFF\xFF\xFF\xFF" | mosquitto_pub -t mendeleev/<address>/setcolor -s

    echo -en "\x01" | mosquitto_pub -t mendeleev/<address>/setmode -s

    head -n 100 updatefile.elf | mosquitto_pub -t mendeleev/<address>/ota -s

**MQTT publish responses:**

    mendeleev/<address>/setcolor/ack (null)

    mendeleev/<address>/setcolor/nack (null)

## Requirements

* [**Mosquitto**](https://mosquitto.org/) - MQTT broker
* [**libmosquitto**](https://mosquitto.org/man/libmosquitto-3.html) - MQTT client library
* [**libmendeleev**](http://libmodbus.org/) - Mendeleev library
* **GCC + Make** - GNU C Compiler and build automation tool

## Installing

Clone the repository:

    git clone git@github.com:area3001/mqtt2mendeleev.git

Go to the repository:

    cd mqtt2mendeleev

Build the program:

    make

## Running

Run the program:

    ./mqtt2mendeleev -h broker.ip:port /dev/ttyRS485

## Authors

* [**Bert Outtier**](https://github.com/bertouttier) - Initial work

## License

This project is licensed under the [**MIT License**](https://opensource.org/licenses/MIT/) - see the [**LICENSE**](LICENSE) file for details.
