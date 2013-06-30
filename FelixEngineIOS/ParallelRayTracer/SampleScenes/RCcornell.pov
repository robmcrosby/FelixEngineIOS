// Cornell Box
// Adapted from Ryan Schmitt's version
// Author: Robert Crosby

camera {
  location  <0, 5, 15.5>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 5, 0>
}

//light_source {<0, 5, 15> color rgb <0.5, 0.5, 0.5>}
light_source {<0, 9.5, 0> color rgb <1.5, 1.5, 1.5>}

//Spheres on top of short box
sphere { <0,0,0>, 1.2
   translate <0.7, 4.1, 1.5>
   pigment { color rgbf <1, 1, 1, 0.8> }
   finish {diffuse 0.5 reflection 0.5 refraction 1.0 ior 1.3333}
}

sphere { <0,0,0>, 1
   rotate <0, 0, -20>
   translate <2.8, 4.0, 0.9>
   pigment {
      image_map {tga "Images/grid.tga"}
   }
   finish {diffuse 0.5}
}

//Sphere on the ground
sphere { <0,0,0>, 1.5
   translate <-2.5, 1.5, 2.5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.6 specular 0.1 roughness 0.8}
}

//Short box
box { <-1.5,-1.5,-1.5>, <1.5,1.5,1.5>
   rotate <0, -20, 0>
   translate <1.5, 1.5, 1.5>
   pigment {
      image_map {tga "Images/grid.tga"}
   }
   finish {diffuse 0.5}
}

//Tall box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, 20, 0>
   scale <1.5, 3, 1.5>
   translate <-2, 3, -2>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}


// THE ROOM

// floor @ y=0
triangle { <-5,0,5>, <5,0,-5>, <-5,0,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
triangle { <-5,0,5>, <5,0,5>, <5,0,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}

// right wall
triangle { <5,0,5>, <5,10,5>, <5,0,-5>
   pigment { color rgb <1,0,0> }
   finish {diffuse 0.5}
}
triangle { <5,10,5>, <5,10,-5>, <5,0,-5>
   pigment { color rgb <1,0,0> }
   finish {diffuse 0.5}
}

// left wall
triangle { <-5,0,5>, <-5,0,-5>, <-5,10,-5>
   pigment { color rgb <0,1,0> }
   finish {diffuse 0.5}
}
triangle { <-5,10,5>, <-5,0,5>, <-5,10,-5>
   pigment { color rgb <0,1,0> }
   finish {diffuse 0.5}
}

// back wall
triangle { <5,10,-5>, <-5,10,-5>, <5,0,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
triangle { <5,0,-5>, <-5,10,-5>, <-5,0,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}

// ceiling @ y=10, w/ a 1x1 hole in the middle
//big left part
triangle { <-5,10,5>, <-5,10,-5>, <-1,10,5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
triangle { <-1,10,5>, <-5,10,-5>, <-1,10,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
//big right part
triangle { <5,10,5>, <1,10,5>, <1,10,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
triangle { <5,10,5>, <1,10,-5>, <5,10,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
//little front part
triangle { <-1,10,5>, <1,10,1>, <1,10,5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
triangle { <-1,10,5>, <-1,10,1>, <1,10,1>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
//little back part
triangle { <-1,10,-1>, <1,10,-5>, <1,10,-1>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}
triangle { <-1,10,-1>, <-1,10,-5>, <1,10,-5>
   pigment { color rgb <1,1,1> }
   finish {diffuse 0.5}
}



