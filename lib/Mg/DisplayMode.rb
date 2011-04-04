class DisplayMode

  attr :width, :height, :bits_per_pixel
  alias :bpp :bits_per_pixel

  def initialize w, h, bpp
    @width, @height, @bits_per_pixel = w, h, bpp
  end
end
