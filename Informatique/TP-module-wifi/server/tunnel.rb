require 'socket'
include Socket::Constants
require_relative 'serveur.rb'


# initialize connection with tunnel8266.rb
esp = TCPServer.new 8266
client_esp = esp.accept

# initialize connection with simuDarwin.rb
# darwin = Socket.new(AF_INET, SOCK_STREAM, 0)
# sockaddr = Socket.sockaddr_in(2211, '0.0.0.0')
# darwin.bind(sockaddr)
# darwin.listen(5)
# if not client_darwin.nil?
#     puts "Connected"
#     str = client_darwin.recv( 100 )
#     puts "Darwin sends #{str}"
#     # client_darwin.close
#     client_esp.puts str
#     puts "transfered to esp!"
# end

# loop
loop do
    puts "try connect"
    # try connect to simuDarwin.rb
    client_darwin = connect(2211, '0.0.0.0')
    if not client_darwin.nil?
        puts client_darwin.readline.chomp
        client_darwin.puts "Darwin bien recu"
        client_darwin.close
    end

    puts "try read"
    # try read from tunnel8266.rb
    data = receive(client_esp)
    if data != ""
    # if line = client_esp.recv(100) # Read lines from socket
        puts line
    end

end

# close connection with tunnel8266.rb
esp.close
