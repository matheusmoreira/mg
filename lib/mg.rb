module Mg

  def self.root
    File.expand_path File.join(File.dirname(__FILE__), '..')
  end

  def self.lib
    File.join Mg.root, 'lib'
  end

end

# Require core mg library
require File.join(Mg.lib, 'mg', 'display_mode')

# Require native extension
require File.join(Mg.lib, 'kg')
