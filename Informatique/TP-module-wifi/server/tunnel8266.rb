require 'socket'
require_relative 'serveur.rb'
include Socket::Constants


# initialize connection with tunnel.rb
tunnel = TCPSocket.new 'localhost', 8266
puts "Connected to DarwinOp"

loop do
    # try connect to simu8266.rb
    str = checkSocket(2200, '0.0.0.0')
    if str != ""
        puts "ESP: #{str}"
        client_esp.puts "ESP: #{str}"
    end

    # try read from tunnel.rb
    data = receive(tunnel)
    if data != ""
        puts "Darwin: #{line}"
    end
end

tunnel.close
