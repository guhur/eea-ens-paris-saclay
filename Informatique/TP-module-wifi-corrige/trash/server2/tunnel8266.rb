#!/usr/bin/env ruby

require 'socket'
require_relative 'serveur.rb'
include Socket::Constants


# initialize connection with tunnel.rb
socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(8266, '0.0.0.0')
socket.connect(sockaddr)
puts "Connected to DarwinOp"

loop do
    # try connect to simu8266.rb
    str = checkSocket(2200, '0.0.0.0')
    if str != ""
        puts "ESP: #{str}"
        socket.puts "ESP: #{str}"
    end

    # try read from tunnel.rb
    data = receive(socket)
    if data != ""
        puts "Darwin: #{data}"
    end
end

socket.close
