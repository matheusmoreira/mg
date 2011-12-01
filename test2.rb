#!/usr/bin/env ruby
load './lib/mg.rb'

include Mg

mode = DisplayMode.current

w, h = 300, 300
x, y = mode.w / 2 - w / 2, mode.h / 2 - h / 2

windows = []

2.times do |n|
  windows << (Window.new "Window #{n}", x, y, w, h).tap do |window|
    window.on_key_press do |key|
      puts "#{window.title} - Key pressed: #{key}"
    end

    window.on_close do
      puts "#{window.title} closed"
      window.hide
    end
  end
end

windows.each do |window|
  window.show
  window.x = x
  window.y = y
  window.event_thread.join
end
