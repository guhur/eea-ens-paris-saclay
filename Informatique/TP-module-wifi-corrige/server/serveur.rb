require 'socket'
require 'fileutils'
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


def checkSocketTCP(host, port)
    socket = Socket.tcp(host,port)
    socket.puts "hello"
    socket.close
    # return s
rescue
    puts "error"
    return ""
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



def query(command, port=2211, address = '0.0.0.0')
    socket = Socket.new(AF_INET, SOCK_STREAM, 0)
    sockaddr = Socket.sockaddr_in(port, address)
    socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)
    socket.bind(sockaddr)
    socket.listen(5)
    client_socket, client_addrinfo = socket.accept
    client_socket.puts command
    socket.close
end


# Manage a tunnel to the ESP8266 chip that can be deamonized
# more info on http://codeincomplete.com/posts/ruby-daemons/
class Tunnel
    attr_reader :options, :quit, :esp

    def initialize(options)
        @options = options
        # daemonization will change CWD so expand relative paths now
        options[:logfile] = File.expand_path(logfile) if logfile?
        options[:pidfile] = File.expand_path(pidfile) if pidfile?
        if logfile?
            redirect_output
        elsif daemonize?
            suppress_output
        end
    end

    def esp8266
        @esp = Socket.tcp('192.168.31.87', 1336)
        if !@esp.nil?
           puts "Connected to ESP8266"
        else
           puts "Error to connect to ESP8266"
        end
    end

    def daemonize?
        options[:daemonize]
    end

    def logfile
        options[:logfile]
    end

    def pidfile
        options[:pidfile]
    end

    def logfile?
        !logfile.nil?
    end

    def pidfile?
        !pidfile.nil?
    end

    # launch tunnel
    def run!
        check_pid
        daemonize if daemonize?
        write_pid
        trap_signals

        esp8266

        while !quit
            # try read any command from HTTP server
            str = checkSocket(2211, '0.0.0.0')
            if str != ""
                puts "NWEB: #{str}"
                @esp.puts "#{str}"
            end

            # try read from ESP866
            data = receive(@esp)
            if data != ""
                puts "ESP8266: #{data}"
            end
        end

        puts "Quitting..."
    end

    def write_pid
        if pidfile?
            begin
                File.open(pidfile, ::File::CREAT | ::File::EXCL | ::File::WRONLY){|f| f.write("#{Process.pid}") }
                at_exit { File.delete(pidfile) if File.exists?(pidfile) }
            rescue Errno::EEXIST
                check_pid
                retry
            end
        end
    end


    def check_pid
        if pidfile?
            case pid_status(pidfile)
            when :running, :not_owned
                puts "A server is already running. Check #{pidfile}"
                exit(1)
            when :dead
                File.delete(pidfile)
            end
        end
    end

    def pid_status(pidfile)
        return :exited unless File.exists?(pidfile)
        pid = ::File.read(pidfile).to_i
        return :dead if pid == 0
        Process.kill(0, pid)      # check process status
        :running
    rescue Errno::ESRCH
        :dead
    rescue Errno::EPERM
        :not_owned
    end

    def daemonize
      exit if fork
      Process.setsid
      exit if fork
      Dir.chdir "/"
    end

    def redirect_output
        FileUtils.mkdir_p(File.dirname(logfile), :mode => 0755)
        FileUtils.touch logfile
        File.chmod(0644, logfile)
        $stderr.reopen(logfile, 'a')
        $stdout.reopen($stderr)
        $stdout.sync = $stderr.sync = true
    end

    def suppress_output
        $stderr.reopen('/dev/null', 'a')
        $stdout.reopen($stderr)
    end

    def trap_signals
        trap(:QUIT) do   # graceful shutdown of run! loop
            @quit = true
            @esp.close
        end
    end

end
