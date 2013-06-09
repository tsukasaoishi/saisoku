class Saisoku
  class HtmlEscape
    class << self
      HTML_ESCAPE = { '&' => '&amp;',  '>' => '&gt;',   '<' => '&lt;', '"' => '&quot;', "'" => '&#39;' }

      def escape(str)
        if str.respond_to?("html_safe?")
          str.html_safe? ? str.to_s.html_safe : index.replace(str.to_s).html_safe
        else
          index.replace(str.to_s)
        end
      end

      private

      def index
        @index ||= ::Saisoku.new(HTML_ESCAPE)
      end
    end
  end
end
