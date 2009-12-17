#!/usr/bin/ruby

def randbool
  rand(2) == 1
end

game_length = (ARGV[0] || 120).to_i
velocity = 0.03
fps = 30
pos_world_depth = (fps * velocity * game_length)
world_depth = -(fps * velocity * game_length)

puts \
"
#geometry

{
    objgeometry
    name ball
    bs sphere
    path geometry/ball.obj
}

{
    objgeometry
    name asteroid
    bs sphere
    path geometry/ball.obj
}

{
    objgeometry
    name floor
    boundingbox -12 -13 -5 12 0 5
}

{
    objgeometry
    name land
    boundingbox 12 0 0 -12  -5
    path geometry/land.obj
}

gravity 0 -9.8 0

world_depth #{world_depth}

"

puts "#models"
puts \
"
##############################
###### WORLD
##############################

{
    wall
    type left
    move -5
}

{
    wall
    type right
    move 5  
}

{
    wall
    type top
    move 10
}

{
    wall
    type bottom
    move -10 
}

{
    wall
    type front
    move 0   
}

{
    wall
    type back
    move -20
}
"

puts \
"
##############################
###### TERRAIN
##############################

{
    environmentmodel
    geometry floor
    scale #{pos_world_depth} 1 1
    rotate 0 90 0
    translate 0 -2 0 
}


"

spacing = -5
(0..((world_depth/spacing).abs).to_i+5).each do |depth|
  dz = spacing*depth

  puts \
"{
    environmentmodel
    geometry land
    scale 1 1 1
    rotate 0 90 0
    translate 0 -2 #{dz} 
    color 0.7 0.4 0.4
}\n\n"
end


puts \
"
##############################
###### OBSTACLES
##############################

"

spacing = -1
(5..((world_depth/spacing).abs).to_i).each do |depth|
  geometry = 'ball'
  
  dx = rand * 1.5
  dy = rand - 0.2
  dz = spacing*depth
  
  dx *= (randbool ? 1 : -1)
  dy *= (randbool ? 1 : -1)
  dz += rand*(randbool ? 1 : -1)
  
  # force some of these to be dead in the center
  if depth % 4 == 0
    dx = dy = 0
  end
  
  scale = 0.05 + rand(5)*0.08
  
  r = rand
  g = rand
  b = rand
  if r+g+b < 0.1
    r = 1
  end
  
  puts \
"{
    passivemodel
    geometry #{geometry}
    scale #{scale} #{scale} #{scale}
    translate #{dx} #{dy} #{dz}
    color #{r} #{g} #{b}
}\n\n"
end
