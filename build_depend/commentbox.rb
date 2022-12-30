#!/usr/bin/env ruby

class ::String
	def justify_to(length)
		self + (' ' * (length - self.length))
	end
end
class ::Integer
	def is_even?
		self % 2 == 0
	end
end

class CommentBox
	def initialize (itext = nil, padding = 4)
		if itext.is_a? String then @text = itext.split "\n"
		elsif itext.is_a? Array then @text = itext end

		if @text.size.is_even? then throw 'Text must have an odd number of lines' end
		@max_line_length = @text.map(&:length).max
		@padding = padding
		self.to_s
	end
	def to_s
		'/*' << ('*' * (@max_line_length + @padding * 2)) << "-/\n\\ " << (' ' * (@max_line_length + @padding * 2)) << " \\\n"\
		<< (0..@text.size - 1).map { |line| fmt_text_line(line) << "\n" }.join\
		<< "\\ " << (' ' * (@max_line_length + @padding * 2)) << " \\\n" << '/-' << ('*' * (@max_line_length + @padding * 2)) << "*/"
	end
	def text=(text)
		self.to_s
	end
private
	def padding_char (char); char * @padding end
	def fmt_text_line(line)
		ret = padding_char(' ') << @text[line].justify_to(@max_line_length) << padding_char(' ')
		line.is_even? ? '/ ' << ret << ' /' : '\\ ' << ret << ' \\'	
	end
end