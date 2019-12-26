const net = require('net')

const SerialPort = require('serialport')

const port = new SerialPort('/dev/cu.usbmodem141301', { baudRate: 9600, autoOpen: false })
port.on('data', data => {
  console.log('Sent:', data.toString().replace("\r", "<CR>").replace("\n", "<LF>"))
  socket.write(data.toString())
})

const socket = new net.Socket()
socket.on('data', function(data) {
  console.log('Rcvd:', data.toString().replace("\r", "<CR>").replace("\n", "<LF>"))
})

port.open(() => {
  setTimeout(() => {
    port.write('+++')
  }, 3000)
})

socket.connect(6379, 'localhost');
