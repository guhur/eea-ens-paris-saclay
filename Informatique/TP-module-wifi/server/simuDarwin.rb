require 'socket'
include Socket::Constants
require_relative 'serveur.rb'


socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(2211, '0.0.0.0')
socket.bind(sockaddr)
socket.listen(5)
begin
    client_socket, client_sockaddr = socket.accept_nonblock
rescue Errno::EAGAIN, Errno::ECONNABORTED, Errno::EINTR, Errno::EWOULDBLOCK
    IO.select([socket])
    retry
end
# puts client_socket.readline.chomp
client_socket.puts "hi from the simulated HTTP server"
client_socket.close
socket.close


#
# dts = TCPServer.new('0.0.0.0', 2211)
# Thread.start(dts.accept) do |s|
#     print(s, " is accepted\n")
#     s.write(Time.now)
#     print(s, " is gone\n")
#     s.close
# end

# print "Input text: "
# input = gets.strip
# socket = TCPSocket.new( "0.0.0.0", 2211 )
# socket.accept
#
#
# socket.puts input
# puts "The server said, '#{socket.readline.chomp}'"
# socket.close
