class Mg::Window

  SUPPORTED_EVENTS = %w(close key_press key_release).map!(&:to_sym).freeze

  attr_reader :event_thread

  def show
    self.visible = true
  end

  def hide
    self.visible = false
  end

  SUPPORTED_EVENTS.each do |event|
    class_eval <<-METHOD
      def on_#{event} &block
        @#{event}_handler = block
      end
    METHOD
  end

end
