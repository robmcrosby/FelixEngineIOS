//
//  Shader.fsh
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
