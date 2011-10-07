require 'mkmf'
require 'rbconfig'

case RbConfig::CONFIG['host_os']
  when /linux/
    $defs << '-DMG_PLATFORM_LINUX'
    if have_library 'X11'
      $defs << '-DMG_PLATFORM_LINUX_X11'
      have_library 'Xrandr'
    end
  when /win/
    $defs << '-DMG_PLATFORM_WINDOWS'
  else
    puts 'Platform not supported'
    exit false
end

create_makefile 'kg'
