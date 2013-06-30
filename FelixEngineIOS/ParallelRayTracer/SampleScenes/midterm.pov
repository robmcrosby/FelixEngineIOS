camera {
   location  <0, 0, 14>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <0.75, 0.75, 0.75>}

sphere { <0, 0, 0>,1.5
   pigment { color rgbf <1.0, 1.0, 1.0, 1.0>}
   finish {ambient 0.1 diffuse 0.4  refraction 1.0 ior 1.33}
}

plane {<0, 1, 0>, -4
   pigment {color rgb <0.2, 0.2, 0.6>}
   finish {ambient 0.01 diffuse 0.4}
}

plane {<0, 0, 1>, -20
   pigment {color rgb <0.6, 0.2, 0.4>}
   finish {ambient 0.01 diffuse 0.4 reflection 0.5}
}
