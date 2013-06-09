$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

require 'saisoku.bundle'
require "saisoku/version"
require "saisoku/html_escape"

class Saisoku
  def add(hash)
    hash.each do |match, replacement|
      set_match_and_replacement(match, replacement)
    end
  end
end
