require 'socket'
include Socket::Constants
require_relative 'serveur.rb'

# initialize connection with tunnel8266.rb
include Socket::Constants
esp = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(8266, '0.0.0.0')
# esp.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)
esp.bind(sockaddr)
esp.listen(5)
client_esp, client_addrinfo = esp.accept
puts "Connected to ESP8266"

#
# esp = TCPServer.new 8266
# client_esp = esp.accept

# loop do
#     data = receive(client_esp)
#     if data != ""
#         puts "I only received 20 bytes '#{data}'"
#     end
#
# end

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
        puts "ESP8266: #{data}"
    end
end

esp.close
