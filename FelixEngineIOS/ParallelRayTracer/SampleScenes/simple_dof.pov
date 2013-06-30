
camera {
  location <4, 5, 24>
  up <0, 1, 0>
  right <1.33, 0, 0>
  look_at <-4, 0, 1>
  
  focal_point <2, 2, -4>
  aperture 0.2
  blur_samples 128
}

light_source { <15, 25, 10> color rgb <1.0, 1.0, 1.0> }

plane { <0, 1, 0>, 0
    pigment {color rgb <0.8, 0.8, 0.8>}
    finish { ambient 0.2 diffuse 0.8 }
}

sphere {<6, 2, -45>, 2
  pigment { color rgbf <0.8, 0.2, 0.8> }
  finish { ambient 0.2 diffuse 0.8 }
}

sphere {<3.5, 2, -20>, 2
  pigment { color rgbf <0.8, 0.8, 0.2> }
  finish { ambient 0.2 diffuse 0.8 }
}

sphere {<2, 2, -4>, 2
  pigment { color rgbf <0.2, 0.8, 0.2> }
  finish { ambient 0.2 diffuse 0.8 }
}

sphere {<1, 2, 9>, 2
  pigment { color rgbf <0.8, 0.2, 0.2> }
  finish { ambient 0.2 diffuse 0.8 }
}

sphere {<0, 2, 18>, 2
  pigment { color rgbf <0.2, 0.2, 0.8> }
  finish { ambient 0.2 diffuse 0.8 }
}