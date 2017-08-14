require 'socket'
require 'fileutils'
include Socket::Constants


def receive(socket)
end


def checkSocketTCP(host, port)
end





def checkSocket(port, host)
end



def query(command, port=2211, address = '0.0.0.0')
end


# Manage a tunnel to the ESP8266 chip that can be deamonized
# more info on http://codeincomplete.com/posts/ruby-daemons/
class Tunnel
    attr_reader :options, :quit, :esp

    def initialize(options)
    end

    def esp8266
    end

    # launch tunnel
    def run!
    end


end
