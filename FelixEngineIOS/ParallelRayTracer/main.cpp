//
//  main.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 4/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include <iostream>

#include "RayTracer.h"

#include <float.h>
#include "Utils/Ray.h"
#include "Item.h"

using namespace std;

int main(int argc, const char *argv[]) {
   RayTracer rayTracer;
   RayOptions options;
   ivec2 imageSize(640, 480);
   string scenePath("SampleScenes/simple.pov");
   string imagePath("simple.tga");
   int i;
   
   // process the arguments
   for (i = 1; i < argc; ++i) {
      string arg(argv[i]);
      
      if (arg.find("-i") != string::npos && ++i < argc)
         scenePath = argv[i];
      else if (arg.find("-o") != string::npos && ++i < argc)
         imagePath = argv[i];
      else if (arg.find("-s") != string::npos && ++i < argc) {
         arg = argv[i];
         if (arg.find("phong") != string::npos)
            options.shader = SHADER_PHONG;
         else if (arg.find("blinn") != string::npos)
            options.shader = SHADER_BLINN;
         else if (arg.find("gaussian") != string::npos)
            options.shader = SHADER_GAUSSIAN;
      }
      else if (arg.find("-aa") != string::npos && ++i < argc) {
         arg = argv[i];
         if (arg.find("2x2") != string::npos)
            options.aaType = DIST_2X2;
         else if (arg.find("3x3") != string::npos)
            options.aaType = DIST_3X3;
         else if (arg.find("4x4") != string::npos)
            options.aaType = DIST_4X4;
      }
      else if (arg.find("-gi") != string::npos && ++i < argc) {
         options.giSamples = atoi(argv[i]);
         options.giLevels = 3;
      }
      else if (arg.find("-ao") != string::npos && ++i < argc) {
         options.aoSamples = atoi(argv[i]);
         options.shader = SHADER_AO;
      }
      else if (arg.find("-v") != string::npos)
         options.varbose = true;
      else if (arg.find("-p") != string::npos)
         options.parrallel = true;
      else {
         int val = atoi(argv[i]);
         if (val) {
            imageSize.x = val;
            if (++i < argc && (val = atoi(argv[i])))
               imageSize.y = val;
         }
      }
   }
   
   // run the ray tracer
   rayTracer.setScene(scenePath);
   rayTracer.setOptions(options);
   rayTracer.setBufferSize(imageSize);
   rayTracer.clearBuffer();
   rayTracer.renderScene();
   rayTracer.saveBuffer(imagePath);
   
   return 0;
}
