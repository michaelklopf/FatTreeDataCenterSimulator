# Generating experiment inis

#Execute with ruby createIniBorderExp.rb

data = File.readlines("inputBorders.ini")

upper = [10, 20, 40, 60, 80, 160]
lower = [5,	10,	20,	30,	40,	80]
lower2 = [3, 5, 10, 15, 20, 40]
repeats = (1..5).to_a

upper.zip(lower).each do |upper, lower|
  repeats.each do |i|
    data_temp = data.dup
    data_temp.map! {|line| line.gsub(/%SEED%/, "#{i}")}
    data_temp.map! {|line| line.gsub(/%LOW%/, "#{lower}")}
    data_temp.map! {|line| line.gsub(/%UP%/, "#{upper}")}
    name_temp = "#{upper}-#{lower}-#{i}"
    filename = "border#{name_temp}.ini"
    File.open(filename, "a") {|f| f.puts data_temp}
    puts "Created #{filename} with borders up: #{upper}, low: #{lower}, seed: #{i}."
  end
end

=begin
upper.zip(lower2).each do |upper, lower|
  repeats.each do |i|
    data_temp = data.dup
    data_temp.map! {|line| line.gsub(/%SEED%/, "#{i}")}
    data_temp.map! {|line| line.gsub(/%LOW%/, "#{lower}")}
    data_temp.map! {|line| line.gsub(/%UP%/, "#{upper}")}
    name_temp = "#{upper}-#{lower}-#{i}"
    filename = "border#{name_temp}.ini"
    File.open(filename, "a") {|f| f.puts data_temp}
    puts "Created #{filename} with borders up: #{upper}, low: #{lower}, seed: #{i}."
  end
end
=end
