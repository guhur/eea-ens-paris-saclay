require 'socket'
include Socket::Constants


def receive(socket)
    data = ""
    begin # emulate blocking recvfrom
        pair = socket.recvfrom_nonblock(20)
        data = pair[0].chomp
    rescue
    end
    return data
end

def checkSocket(port, host)
    socket = Socket.new(AF_INET, SOCK_STREAM, 0)
    sockaddr = Socket.sockaddr_in(port, host)
    begin # emulate blocking connect
        socket.connect_nonblock(sockaddr)
        rescue IO::WaitWritable
            IO.select(nil, [socket]) # wait 3-way handshake completion
        begin
            socket.connect_nonblock(sockaddr) # check connection failure
        rescue
        end
    ensure
        s = socket.read
        socket.close
    end
    return s
end
