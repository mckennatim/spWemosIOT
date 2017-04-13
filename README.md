# IOT platform apps
## setup
;shroom:humidity temp control for noah -dht11+2relays 1 input, 2output
;temp: outdoor thermometer -dht11, 1 input
;hrc: heated roof control-1 relay, 2inputs, 1output
;timerrelay: timer for plants 1relay 1 output
;irrig: gravity feed water release 1timer output (moisture level input)

* ^ bundle components into 4 sets
* ^ solder connectors on wemos
* ^ test led - make test component
* ^ test relay - create relay component if needed
* test dht11 temp humidity sensor
* copy separate project files

### Understanding the code 
#### test relay
the device understands a command like 
`client.publish('CYURD001/prg', {"id":2,"pro":[[0,0,1]]})` 
to do that and `blank/aclient/rawSb.html`'s 'turnOnOff()' accomplishes that for timer 1 which outputs on/off on `io15d8` pin on the wemos

