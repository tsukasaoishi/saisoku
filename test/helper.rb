require 'rubygems' if !defined?(Gem)
require 'bundler/setup'

require 'saisoku'
require 'minitest/autorun'

$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
