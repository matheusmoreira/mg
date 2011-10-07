class Mg::DisplayMode

  attr :width, :height, :bits_per_pixel

  alias :w   :width
  alias :h   :height
  alias :bpp :bits_per_pixel

  def initialize(width, height, bits_per_pixel)
    @width, @height, @bits_per_pixel = width, height, bits_per_pixel
  end

  def to_s
    "#{width}x#{height}@#{bits_per_pixel}"
  end

end
