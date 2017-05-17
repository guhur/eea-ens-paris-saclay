#!/usr/bin/env ruby

require_relative 'serveur.rb'

options = {
    :pidfile    => "./tunnel.pid",
    :logfile    => "./tunnel.log",
    :daemonize  => true,
    :port       => "1336",
    :address    => "192.168.43.236"
}

Tunnel.new(options).run!
