//
//  ViewController.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class Host;

@interface ViewController : GLKViewController {
@private
   Host *host_;
   CGFloat width_;
   CGFloat height_;
   CGFloat scale_;
}

@property (strong, nonatomic) EAGLContext *context;

@end
