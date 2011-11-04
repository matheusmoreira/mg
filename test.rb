load './lib/mg.rb'

include Mg

mode = DisplayMode.current

w, h = 300, 300
x, y = mode.w / 2 - w / 2, mode.h / 2 - h / 2

window = Window.new 'OpenGL Window', x, y, w, h

window.on_key_press do |key|
  puts "Key pressed: #{key}"
end

window.on_close do
  puts 'Window closed'
  window.hide
end

window.show

window.x = x
window.y = y

window.event_thread.join
