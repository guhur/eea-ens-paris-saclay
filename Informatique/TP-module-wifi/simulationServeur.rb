#!/usr/bin/env ruby

require 'socket'
include Socket::Constants
require_relative 'nweb/serveur.rb'



socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(1234, '0.0.0.0')
socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)
socket.bind(sockaddr)
socket.listen(5)
client_socket, client_addrinfo = socket.accept
print(client_socket, " is accepted\n")
print "Input text: "
input = gets.strip
client_socket.puts input
socket.close
