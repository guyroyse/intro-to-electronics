const net = require('net')

const SerialPort = require('serialport')

const port = new SerialPort('/dev/cu.usbmodem141301', { baudRate: 9600 })
port.on('data', data => {
  console.log('Sent:', data.toString().replace("\r", "<CR>").replace("\n", "<LF>"))
  socket.write(data.toString())
})

const socket = new net.Socket()
socket.on('data', data => {
  console.log('Rcvd:', data.toString().replace("\r", "<CR>").replace("\n", "<LF>"))
  port.write(data.toString());
})

socket.connect(6379, 'localhost');
