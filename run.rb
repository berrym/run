#!/usr/bin/env ruby

if __FILE__ == $PROGRAM_NAME
  if ARGV.empty?
    puts "usage: #{$PROGRAM_NAME} command args"
  else
    Process.spawn(ARGV.join(" "))
  end
end
