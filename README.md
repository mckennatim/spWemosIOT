# IOT platform apps
## todo
Relay is only set when a `readTemp` happens and the temp has changed. Relays also need to be set when a new program resets the hilo limits which happens in `sched:ckAlarms`
## setup
;dht:humidity temp control for noah's shrooms -dht11+2relays 1 input, 2output
;temp: outdoor thermometer -dht11, 1 input
;hrc: heated roof control-1 relay, 2inputs, 1output
;timerrelay: timer for plants 1relay 1 output
;irrig12v: water valve 1timer output (moisture level input)

* ^ bundle components into 4 sets
* ^ solder connectors on wemos
* ^ test led - make test component
* ^ test relay - create relay component if needed
* test dht11 temp humidity sensor
* copy separate project files

### Understanding the code 
#### test relay
software: the device understands a command like 
`client.publish('CYURD001/prg', {"id":2,"pro":[[0,0,1]]})` 
to do that and `blank/aclient/rawSb.html`'s 'turnOnOff()' accomplishes that for timer 1 which outputs on/off on `io15d8` pin on the wemos

hardware: First just manually operate the relay by connecting it to a power supply. Relays work when input us given a path to ground. They need to be driven by a transistor and protected by a diode. Mock up that circuit to check it. 
#### dht test
* duplicate blank as dht
* `sketch/include libraries/manage libraries` and search for dht. Install.
* read DHT from `io14d5` send it out with temp->temp1 and humidity->temp2

## tags
### 02-relay_dht_tests
### 03-adjHeat-onPrgChange
wipes out days program, puts in a hold. If you want to boost for an hour starting in 30 minutes, then the client would have to grab the days program and insert the boost into it. 
### 04-adjHeat-onStateChange
just changes state or hilo for the duration of the current program. Reverts to program control at next program event for that sensor/relay.



