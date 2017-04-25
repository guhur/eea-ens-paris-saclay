require 'socket'
include Socket::Constants


print "Input text: "
input = gets.strip


socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(2200, 'localhost')
socket.connect(sockaddr)
socket.close
