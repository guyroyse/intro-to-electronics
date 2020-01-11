const Five = require("johnny-five")
const Redis = require("redis")

let board = new Five.Board({ repl: true })
let redis = Redis.createClient()


board.on("ready", function() {

  setupOutputPins([2, 3, 4, 5, 6, 7, 8])
  setPwnPins([9, 10, 11])
  setupInputPins([12, 13])
  setupAnalogInputPins(['A0', 'A1', 'A2', 'A3', 'A4', 'A5'])
})


function setupOutputPins(pinNumbers) {

  let leds = pinNumbers.map(number => new Five.Led(number))
  let redisKeys = pinNumbers.map(number => `arduino:pin:${number}`)

  setInterval(() => {
    redis.mget(...redisKeys, (err, values) => {
      values
        .map(value => Number(value))
        .forEach((value, index) => value ? leds[index].on() : leds[index].off())
    })
  }, 200)
}

function setPwnPins(pinNumbers) {

  let leds = pinNumbers.map(number => new Five.Led(number))
  let redisKeys = pinNumbers.map(number => `arduino:pin:${number}`)

  setInterval(() => {
    redis.mget(...redisKeys, (err, values) => {
      values
        .map(value => Number(value))
        .forEach((value, index) => leds[index].brightness(value))
    })
  }, 200)
}

function setupInputPins(pinNumbers) {

  let pins =  pinNumbers.map(number => new Five.Pin(number));
  let redisKeys = pinNumbers.map(number => `arduino:pin:${number}`)

  pins.forEach((pin, index) => {
    pin.read((err, value) => redis.set(redisKeys[index], value))
  })
}

function setupAnalogInputPins(pinNumbers) {

  let pins =  pinNumbers.map(number => new Five.Pin(number));
  let redisKeys = pinNumbers.map(number => `arduino:pin:${number}`)

  pins.forEach((pin, index) => {
    pin.read((err, value) => redis.set(redisKeys[index], value))
  })
}
