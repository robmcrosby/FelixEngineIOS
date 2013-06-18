//
//  ViewController.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "IOSHost.h"

@interface ViewController : GLKViewController {
@private
   IOSHost *host_;
   //AppEngine *appEngine_;
   CGFloat width_;
   CGFloat height_;
   CGFloat scale_;
   //vec2 halfScreen_;
}

@property (strong, nonatomic) EAGLContext *context;

@end
