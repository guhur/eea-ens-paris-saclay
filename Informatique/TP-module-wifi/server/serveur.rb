require 'socket'
include Socket::Constants


def receive(socket)
    begin # emulate blocking recvfrom
        pair = client.recvfrom_nonblock(20)
        data = pair[0].chomp
    rescue
        # we did not receive anything
        data = ""
    end
    return data
end

def connect2(port,host = '0.0.0.0', timeout=0)
    puts "try connection"
    return TCPSocket.new( host, port )
rescue
    return nil
end


# connect with non blocking socket
def connect(port,host = '0.0.0.0', timeout=0)
    addr = Socket.getaddrinfo(host, nil)
    sockaddr = Socket.pack_sockaddr_in(port, addr[0][3])
    socket = nil

    Socket.new(Socket.const_get(addr[0][0]), Socket::SOCK_STREAM, 0).tap do |socket|
        socket.setsockopt(Socket::IPPROTO_TCP, Socket::TCP_NODELAY, 1)
        begin
            socket.connect_nonblock(sockaddr)
        # rescue IO::WaitWritable # time out
        #     puts "waiting"
        #     if timeout != 0
        #         if IO.select(nil, [socket], nil, timeout)
        #             begin
        #                 # Verify there is now a good connection
        #                 puts "good connection ?"
        #                 socket.connect_nonblock(sockaddr)
        #             rescue Errno::EISCONN
        #                 puts "connected !"
        #                 # Good news everybody, the socket is connected!
        #             rescue
        #                 puts "idk"
        #                 socket.puts "try"
        #                 socket.close
        #                 socket = nil
        #             end
        #         else #Connection timeout
        #             puts "time out"
        #             socket.close
        #             socket = nil
        #         end
        #     end
        rescue Errno::EAGAIN, Errno::ECONNABORTED, Errno::EINTR, Errno::EWOULDBLOCK, Errno::EINPROGRESS
            puts "smthg else"
            socket.close
            socket = nil
        end
    end

    return socket
end
