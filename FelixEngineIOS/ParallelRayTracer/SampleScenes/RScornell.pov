// Cornell Box
// Adapted from original
// Author: Ryan Schmitt

camera {
  location  <0, 5, 15.5>
  up        <0,  1,  0>
  right     <1, 0,  0>
  look_at   <0, 5, 0>
}

light_source {<0, 5, 15> color rgb <0.1, 0.1, 0.1>}
light_source {<0, 9.5, 0> color rgb <0.5, 0.5, 0.5>}

//Sphere on top of short box
sphere { <0,0,0>, 1
   scale <1.5, 1.5, 1.5>
   translate <1.5, 4.4, 1.5>
   pigment { color rgbf <1, 1, 1, 0.8> }
   finish { ambient 0 diffuse 0.5 refraction 1.0 ior 1.3333 }
}

//Sphere on the ground
sphere { <0,0,0>, 1
   scale <1.5, 1.5, 1.5>
   translate <-2.5, 1.5, 2.5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.8 }
}

//Short box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, -20, 0>
   scale 1.5
   translate <1.5, 1.5, 1.5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}

//Tall box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, 20, 0>
   scale <1.5, 3, 1.5>
   translate <-2, 3, -2>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}


// SPHERES ON BACK WALL
//row1
sphere { <-4, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <-2, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <0, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <2, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <4, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
//row2
sphere { <-4, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <-2, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <0, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <2, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <4, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
//row3
sphere { <-4, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <-2, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <0, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <2, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <4, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
//row4
sphere { <-4, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <-2, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <0, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <2, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <4, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
//row5
sphere { <-4, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <-2, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <0, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <2, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}
sphere { <4, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.2 diffuse 0.5 specular 1.0 roughness 0.005 reflection 0.1 }
}


// THE ROOM

// floor @ y=0
triangle { <-5,0,5>, <5,0,-5>, <-5,0,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <-5,0,5>, <5,0,5>, <5,0,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}

// right wall
triangle { <5,0,5>, <5,10,5>, <5,0,-5>
   pigment { color rgb <1,0,0> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <5,10,5>, <5,10,-5>, <5,0,-5>
   pigment { color rgb <1,0,0> }
   finish { ambient 0 diffuse 0.5 }
}

// left wall
triangle { <-5,0,5>, <-5,0,-5>, <-5,10,-5>
   pigment { color rgb <0,1,0> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <-5,10,5>, <-5,0,5>, <-5,10,-5>
   pigment { color rgb <0,1,0> }
   finish { ambient 0 diffuse 0.5 }
}

// back wall
triangle { <5,10,-5>, <-5,10,-5>, <5,0,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <5,0,-5>, <-5,10,-5>, <-5,0,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}

// ceiling @ y=10, w/ a 1x1 hole in the middle
//big left part
triangle { <-5,10,5>, <-5,10,-5>, <-1,10,5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <-1,10,5>, <-5,10,-5>, <-1,10,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
//big right part
triangle { <5,10,5>, <1,10,5>, <1,10,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <5,10,5>, <1,10,-5>, <5,10,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
//little front part
triangle { <-1,10,5>, <1,10,1>, <1,10,5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <-1,10,5>, <-1,10,1>, <1,10,1>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
//little back part
triangle { <-1,10,-1>, <1,10,-5>, <1,10,-1>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}
triangle { <-1,10,-1>, <-1,10,-5>, <1,10,-5>
   pigment { color rgb <1,1,1> }
   finish { ambient 0 diffuse 0.5 }
}



