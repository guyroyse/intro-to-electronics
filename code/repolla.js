const five = require("johnny-five")
const redis = require("redis")

let board = new five.Board({ repl: true })
let client = redis.createClient()

let ledPwmPinNumbers = [9, 10, 11]
let ledPinNumbers = [2, 3, 4, 5, 6, 7, 8, 12, 13]

board.on("ready", function() {

  let pwnLeds = ledPwmPinNumbers
    .map(number => new five.Led(number));

  let leds = ledPinNumbers
    .map(number => new five.Led(number));

  this.repl.inject({ leds: leds })

  let pwmKeys = ledPwmPinNumbers.map(number => `arduino:pin:${number}`)
  let keys = ledPinNumbers.map(number => `arduino:pin:${number}`)

  setInterval(() => {

    client.mget(...pwmKeys, (err, values) => {
      values
        .map(value => Number(value))
        .forEach((value, index) => pwnLeds[index].brightness(value))
    })
    
    client.mget(...keys, (err, values) => {
      values
        .map(value => Number(value))
        .forEach((value, index) => {
          if (value) {
            leds[index].on()
          } else {
            leds[index].off()
          }
        })
    })

  }, 200)

})
