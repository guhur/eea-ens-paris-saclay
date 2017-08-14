#!/usr/bin/env ruby

require_relative 'serveur.rb'

options = {
    :pidfile    => "./tunnel.pid",
    :logfile    => "./tunnel.log",
    :daemonize  => true,
    :port       => "1336",
    :address    => "192.168.31.87"
}

Tunnel.new(options).run!
