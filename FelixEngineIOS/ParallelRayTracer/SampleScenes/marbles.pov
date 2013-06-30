// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
      //location  <6, 5, 6>
      location  <1.2, 2.2, 10>
      up        <0,  1,  0>
      right     <1.33333, 0,  0>
      look_at   <0, 0, 0>
}


light_source {<-5, 8, 20> color rgb <0.6, 0.2, 0.2>}

light_source {<5, 10, 20> color rgb <0.2, 0.6, 0.2>}

light_source {<30, 500, -30> color rgb <.4, .4, .4>}

sphere { <0, 1, 5>, 1
   pigment { color rgb <.5, .9, .7>}
   finish {ambient 0.0 diffuse 0.1 specular 0.9 roughness .2 reflection 0.4 refraction 0.6 ior 1.33}
   translate <0, 0, 0>
}

sphere { <2.5, 1, 0>, 1
   pigment { color rgb <0.6, 0.2, 0.2>}
   finish {ambient 0.2 diffuse 0.8 specular 0.6 roughness 0.2}
   translate <0, 0, 0>
}

sphere { <2.7, 1, -3>, 1
   pigment { color rgb <.3, 0.9, 0.3>}
   finish {ambient 0.2 diffuse 0.8 specular 0.6 roughness 0.2}
   translate <0, 0, 0>
}

sphere { <0, 1, -6>, 1
   pigment { color rgb <.8, 0.5, 0.6>}
   finish {ambient 0.2 diffuse 0.8 specular 0.6 roughness 0.2 reflection 0.6}
   translate <0, 0, 0>
}

sphere { <-2.5, 1, 0>, 1
   pigment { color rgb <0.4, 0.3, 0.9>}
   finish {ambient 0.2 diffuse 0.8 specular 0.6 roughness 0.2}
   translate <0, 0, 0>
}

sphere { <-2.7, 1, -3>, 1
   pigment { color rgb <.8, 0.8, 0.2>}
   finish {ambient 0.2 diffuse 0.8 specular 0.6 roughness 0.2}
   translate <0, 0, 0>
}

plane {<0, 1, 0>, 0
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.4 diffuse 0.9}
}

