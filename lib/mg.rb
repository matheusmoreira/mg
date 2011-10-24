module Mg

  def self.root
    File.expand_path File.join(File.dirname(__FILE__), '..')
  end

  def self.lib
    File.join Mg.root, 'lib'
  end

end

# Require native extension
require File.join Mg.lib, 'mg', 'kg'

# Require core mg library
require File.join Mg.lib, 'mg', 'display_mode'
require File.join Mg.lib, 'mg', 'window'
