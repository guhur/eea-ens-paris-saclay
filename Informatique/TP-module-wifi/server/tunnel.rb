require 'socket'
include Socket::Constants
require_relative 'serveur.rb'

# initialize connection with tunnel8266.rb
esp = TCPServer.new 8266
client_esp = esp.accept
puts "Connected to ESP8266"

loop do
    # try connect to simuDarwin.rb
    str = checkSocket(2211, '0.0.0.0')
    if str != ""
        puts "Darwin: #{str}"
        client_esp.puts "Darwin: #{str}"
    end

    # try read from tunnel8266.rb
    data = receive(client_esp)
    if data != ""
        puts "ESP8266: #{line}"
    end
end

esp.close
