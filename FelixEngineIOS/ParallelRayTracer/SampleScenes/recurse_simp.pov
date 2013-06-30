// csc 473 reflection and refraction
camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.5, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-5, 3, 0> color rgb <0.3, 0.5, 0.3>}
light_source {<5, 10, 5> color rgb <0.3, 0.4, 0.5>}

// floor
plane {<0, 1, 0>, -4
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.4 diffuse 0.2 reflection 0.2}
  translate <0, -1, 0>
}

// left wall
plane {<1, 0, 0>, -8
  pigment {color rgb <0.8, 0.2, 0.2>}
  finish {ambient 0.4 diffuse 0.2 reflection 0.2}
  rotate <0, 30, 0>
}

// back wall
plane {<0, 0, -1>, 30
  pigment {color rgb <0.8, 0.4, 0.2>}
  finish {ambient 0.4 diffuse 0.2 reflection 0.2}
  rotate <0, 30, 0>
}

// back wall on the right
plane {<0, 0, -1>, 30
  pigment {color rgb <0.0, 0.2, 0.2>}
  finish {ambient 0.4 diffuse 0.8 reflection 0.2}
  rotate <0, -20, 0>
}

sphere { <0, 0, 0>, 2
  pigment {color rgbf <1.0, 1.0, 1.0, 0.8>}
  finish {ambient 0.2 diffuse 0.8 refraction 1.0 ior 1.333}
  translate <5.5, -3, -1>
}

