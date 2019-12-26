const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')

const port = new SerialPort('/dev/cu.usbmodem141301', { baudRate: 9600, autoOpen: false })
const parser = new Readline()

port.pipe(parser)

parser.on('data', line => console.log(`> ${line}`))

port.open(() => {
  setTimeout(() => {
    port.write('+++')
  }, 5000)
})
