//----------------------------------------------
//--Exported with POV-Ray exporter for Blender--
//----------------------------------------------

#version 3.7;

//--CUSTOM CODE--


//--Global settings and background--

global_settings {
    assumed_gamma 1.0
    max_trace_level 5
    ambient_light rgb<0, 0, 0>
}

background {rgbt<0.0509, 0.0509, 0.0509, 0>}

//--Cameras--

#declare camLocation  = <7.481132, 5.343666, 6.507640>;
#declare camLookAt = <-27.098163, 46.688390, -0.903519>;
camera {
    location  <0, 0, 0>
    look_at  <0, 0, -1>
    right <-1.7777777777777777, 0, 0>
    up <0, 1, 0>
    angle  49.134343
    rotate  <-27.098163, 46.688390, -0.903519>
    translate <7.481132, 5.343666, 6.507640>
}

//--Lamps--

light_source {
    < 0,0,0 >
    color rgb<2, 2, 2>
    fade_distance 14.999991
    fade_power 2
    matrix <-0.290865, -0.055189, -0.955171,  -0.771101, 0.604525, 0.199883,  0.566393, 0.794672, -0.218391,  4.076245, 5.903862, -1.005454>
}
#declare lampTarget1= vrotate(<-4.076,-1.005,-5.904>,<0.6503,0.05522,1.866>);

//--Material Definitions--

#declare shader_Default = finish {  //translation of spec and mir levels for when no map influences them
    diffuse 0.8
    phong 70.0
}

#declare shader_Material = finish {  //translation of spec and mir levels for when no map influences them
    brilliance 1.8
    phong 0.5
    phong_size 25.2
    diffuse 0.8 0
    ambient 1
    emission 0
    conserve_energy
}


//--Mesh objects--

#declare DATACube =
mesh2 {
    vertex_vectors {
        8,
        <1.000000, 1.000000, -1.000000>,
        <1.000000, -1.000000, -1.000000>,
        <-1.000000, -1.000000, -1.000000>,
        <-1.000000, 1.000000, -1.000000>,
        <1.000000, 0.999999, 1.000000>,
        <0.999999, -1.000001, 1.000000>,
        <-1.000000, -1.000000, 1.000000>,
        <-1.000000, 1.000000, 1.000000>
    }
    normal_vectors {
        6,
        <0.000000, 0.000000, -1.000000>,
        <-0.000000, -1.000000, -0.000000>,
        <1.000000, -0.000000, 0.000000>,
        <-1.000000, 0.000000, -0.000000>,
        <0.000000, 1.000000, 0.000000>,
        <0.000000, -0.000000, 1.000000>
    }
    texture_list {
        1
        texture {
            pigment {rgbft<0.8, 0.8, 0.8, 0, 0>}
            finish {shader_Material}
        }
    }
    face_indices {
        12,
        <0,1,2>, 0,0,0,
        <0,2,3>, 0,0,0,
        <4,7,6>, 0,0,0,
        <4,6,5>, 0,0,0,
        <0,4,5>, 0,0,0,
        <0,5,1>, 0,0,0,
        <1,5,6>, 0,0,0,
        <1,6,2>, 0,0,0,
        <2,6,7>, 0,0,0,
        <2,7,3>, 0,0,0,
        <4,0,3>, 0,0,0,
        <4,3,7>, 0,0,0
    }
    normal_indices {
        12,
        <0,0,0>,
        <0,0,0>,
        <5,5,5>,
        <5,5,5>,
        <2,2,2>,
        <2,2,2>,
        <1,1,1>,
        <1,1,1>,
        <3,3,3>,
        <3,3,3>,
        <4,4,4>,
        <4,4,4>
    }
    interior {
        ior 1.000000
        caustics 0.1
    }
    photons{    }
    radiosity { 
        importance   1 
    }
}
//----Blender Object Name:OBCube----
object { 
    DATACube
    matrix <1.000000, 0.000000, 0.000000,  0.000000, -0.000000, -1.000000,  0.000000, 1.000000, -0.000000,  0.000000, 0.000000, 0.000000>

}
