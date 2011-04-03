require 'mkmf'
require 'rbconfig'

case RbConfig::CONFIG['host_os']
  when /linux/
    $defs << '-DMG_PLATFORM_LINUX'
    $defs << '-DMG_PLATFORM_LINUX_X11' if have_library 'X11', 'XOpenDisplay'
  when /win/
    $defs << '-DMG_PLATFORM_WINDOWS'
  else
    puts 'Platform not supported'
    exit false
end

dir_config('Mg')

create_makefile 'Mn'
