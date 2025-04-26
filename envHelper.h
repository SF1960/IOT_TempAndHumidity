
#include "globals.h"

/*******************************************************************************
*
* Set up the DHT11
*
*******************************************************************************/

// Objects
DHTesp dht;                                       // Initialize the sensor
ComfortState cf;                                  // ComfortState is returned by the DHT11 sensor

namespace sensor{

  void setup(){

    // start DHT11 comms protocol
    Wire.begin();

    // set up DHT11 object
    dht.setup(dhtPin, DHTesp::DHT11);

  }


}