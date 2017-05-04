#!/usr/bin/env ruby

require_relative 'serveur.rb'

f = File.basename(__FILE__, ".cgi")
query f
