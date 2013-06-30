camera {
  location  <0, 0, 7>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<0, 0, 5> color rgb <2, 2, 2>}

triangle {
   <-1, -1, 0>, <-1, 1, 0>, <1, -1, 0>
   uv_vectors <0, 0>, <0, 1>, <1, 0>
   pigment {
      image_map {tga "Images/grid.tga"}
   }
   finish {ambient 0.8 diffuse 0.8}
   scale <10, 10, 1>
   rotate <-90, 0, 0>
   translate <0, -3, -10>
}

triangle {
   <-1, 1, 0>, <1, 1, 0>, <1, -1, 0>
   uv_vectors <0, 1>, <1, 1>, <1, 0>
   pigment {
      image_map {tga "Images/grid.tga"}
   }
   finish {ambient 0.8 diffuse 0.8}
   scale <10, 10, 1>
   rotate <-90, 0, 0>
   translate <0, -3, -10>
}