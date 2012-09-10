#!/usr/bin/ruby

filename = ARGV[0]
n = ARGV[1].to_i

f = File.open("tests/data/" + filename, "w")
f.puts n

for i in 0 ... n
  x = rand(1000)
  y = rand(1000)
  z = rand(1000) 
  f.puts "#{x} #{y} #{z} #{i} #{2 * i}"
end
f.close

