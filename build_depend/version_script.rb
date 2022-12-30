
require 'json'; require 'date'
unless defined? $info_padding; $info_padding = 2; end

class ::String
	def justify_to(length)
		self + (' ' * (length - self.length))
	end
	def justify
		justify_to $max_key_length
	end
	def center_justify_to(length)
		(' ' * ((length - self.length) / 2)) << self << (' ' * ((length - self.length) / 2))
	end
end
class ::Integer; def is_even?; self % 2 == 0 end end

class VersionInfo < Hash
	attr_reader :header_info, :info_size

	def initialize (filename)
		@whole_file_parse = JSON.parse File.read(filename)
		super(); self.merge! @whole_file_parse['defines']
		@header_info = @whole_file_parse['header_info']
		@info_size = @header_info.size - 1
		set_max_length; increment_build_num; set_build_date; set_version_str; set_politeness; update_version_json; quotify
	end
	def header_guard
		@whole_file_parse['header_guard'] ? "\n#ifndef VERSION_H\n#define VERSION_H\n" : nil
	end
	def info_line(num, allignment = :left)
		line = if allignment == :left
		           (' ' * $info_padding) << (self.header_info[num].to_s.justify_to ($info_length + $info_padding))
			   elsif allignment == :center
			       (' ' * $info_padding) << (self.header_info[num].to_s.center_justify_to ($info_length + $info_padding))
		       end
		(num.is_even? ? '\\' << line << '\\' : '/' << line << '/')
	end

private
	@whole_file_parse = Hash.new
	def increment_build_num
		self['BUILD_NUM'] = @whole_file_parse['build_num_format_string'] % (self['BUILD_NUM'].to_i + 1).to_s
	end
	def set_build_date
		self['BUILD_DATE'] = Date.today.strftime @whole_file_parse['build_date_format_string']
	end
	def set_version_str
		self.merge! 'VERSION_STR' => self['VERSION_MAJOR'].to_s << '.'\
			<< (@whole_file_parse['full_version_format_string'] % self['VERSION_MINOR'].to_s)
	end
	def set_max_length
		$max_key_length = self.keys.map(&:length).max
		$info_length = self.header_info.map(&:length).max
	end
	def set_politeness
		if @whole_file_parse['polite']
			self['POLITE'] = ''
		else
			if self['POLITE'] then self.delete 'POLITE' end
		end
	end
	def update_version_json;
		@whole_file_parse['defines'] = self
		File.open('version.json', 'w') { |f| f.write JSON.pretty_generate(@whole_file_parse) }
	end
	def quotify
		self.transform_values! { |value| value.is_a?(String) && value != "" ? '"' << value << '"' : value }
	end
end

def char_line(char); char * ($info_length + ($info_padding * 2) - 2); end

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