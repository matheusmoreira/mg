module Mg
  class DisplayMode

    attr :width, :height, :bits_per_pixel

    alias :w :width
    alias :h :height
    alias :bpp :bits_per_pixel

    def initialize w, h, bpp
      @width, @height, @bits_per_pixel = w, h, bpp
    end

    def to_s
      "#{width}x#{height}@#{bits_per_pixel}"
    end
  end
end
