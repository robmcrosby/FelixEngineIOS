// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-5, 10, 10> color rgb <0.8, 0.8, 0.8>}
light_source {<100, 100, 100> color rgb <0.8, 0.2, 0.2>}

sphere { <0, 0, 0>, 3
   pigment {color rgb <0.8, 0.2, 0.2>}
   finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.02}
   rotate <-45, 0, 0>
   translate <0, 0, 0>
}

plane {<0, 1, 0>, -4
   pigment {color rgb <0.2, 0.2, 0.8>}
   finish {ambient 0.4 diffuse 0.8}
}