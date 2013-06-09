class Saisoku
  class HtmlEscape
    class << self
      HTML_ESCAPE = { '&' => '&amp;',  '>' => '&gt;',   '<' => '&lt;', '"' => '&quot;', "'" => '&#39;' }

      def escape(str)
        if str.respond_to?("html_safe?")
          str.html_safe? ? str.to_s : saisoku.replace(str.to_s).html_safe
        else
          saisoku.replcae(str.to_s).html_safe
        end
      end

      private

      def saisoku
        @saisoku ||= ::Saisoku.new(HTML_ESCAPE)
      end
    end
  end
end
