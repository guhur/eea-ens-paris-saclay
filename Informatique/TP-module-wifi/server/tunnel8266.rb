

# require 'socket'
# include Socket::Constants
#
# socket = Socket.new(AF_INET, SOCK_STREAM, 0)
# sockaddr = Socket.sockaddr_in(8266, '0.0.0.0')
# begin
#   socket.connect_nonblock(sockaddr)
# rescue Errno::EINPROGRESS
#   IO.select(nil, [socket])
#   begin
#     socket.connect_nonblock(sockaddr)
#   rescue Errno::EINVAL
#     retry
#   rescue Errno::EISCONN
#   end
# end
# socket.write("hi from the client\n")
# results = socket.read
# puts results
# socket.close
#

require 'socket'
require_relative 'serveur.rb'
include Socket::Constants

# initialize connection with tunnel.rb
tunnel = TCPSocket.new 'localhost', 8266



# loop
loop do
    puts "try connect"
    # try connect to simu8266.rb
    client = connect(2200, '0.0.0.0')
    if not client.nil?
        puts client.readline.chomp
        client.puts "Darwin bien recu"
        client.close
    end


    # try read from tunnel8266.rb
    puts "try read"
    # if line = tunnel.recv(100) # Read lines from socket
    data = receive(tunnel)
    if data != ""
        puts data
    end

end
#
# # loop
# loop do
#     begin
#         # try connect to simu8266.rb
#         puts "try connect"
#         client_8266, client_sockaddr = s8266.accept_nonblock
#         puts "connected ? #{client_8266}"
#     rescue Errno::EAGAIN, Errno::ECONNABORTED, Errno::EINTR, Errno::EWOULDBLOCK
#         IO.select([s8266])
#
#         # try read from tunnel.rb
#         puts "try read"
#         if line = tunnel.gets # Read lines from socket
#             puts line
#         end
#         retry
#     end
#     puts "try read client"
#     if client_8266.nil?
#         puts client_8266.readline.chomp
#         client_8266.puts "ESP bien recu"
#         client_8266.close
#     end
# end

# close connection with tunnel.rb
tunnel.close
