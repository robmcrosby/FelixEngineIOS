//
//  ViewController.m
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#import "ViewController.h"
#import "IOSHost.h"

@implementation ViewController

@synthesize context = _context;

- (void)dealloc {
   [_context release];
   [super dealloc];
}

- (void)viewDidLoad {
   [super viewDidLoad];
   
   self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
   
   if (!self.context) {
      NSLog(@"Failed to create ES context");
   }
   
   [self.view setMultipleTouchEnabled:YES];
   
   GLKView *view = (GLKView *)self.view;
   view.context = self.context;
   view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
   
   [EAGLContext setCurrentContext:self.context];
   
   //discover the device
   DEV_TYPE deviceType = DEV_PHONE;
   NSString * device = [UIDevice currentDevice].model;
   if ( [device hasPrefix:@"iPad"] )
      deviceType = DEV_TABLET;
   
   //get screen bounds
   if (self.interfaceOrientation == UIInterfaceOrientationPortrait ||
       self.interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
      width_ = [UIScreen mainScreen].bounds.size.width;
      height_ = [UIScreen mainScreen].bounds.size.height;
   }
   else {
      width_ = [UIScreen mainScreen].bounds.size.height;
      height_ = [UIScreen mainScreen].bounds.size.width;
   }
   scale_ = [UIScreen mainScreen].scale;

   host_ = IOSHost::CreateIOSHost(deviceType, width_, height_, scale_);
}

- (void)viewDidUnload {
   [super viewDidUnload];
   
   [EAGLContext setCurrentContext:self.context];
   
   if ([EAGLContext currentContext] == self.context) {
      [EAGLContext setCurrentContext:nil];
   }
   self.context = nil;
}

- (void)didReceiveMemoryWarning {
   [super didReceiveMemoryWarning];
   host_->lowMemory();
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
   if (toInterfaceOrientation == UIInterfaceOrientationPortrait ||
       toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
      width_ = [UIScreen mainScreen].bounds.size.width;
      height_ = [UIScreen mainScreen].bounds.size.height;
   }
   else {
      width_ = [UIScreen mainScreen].bounds.size.height;
      height_ = [UIScreen mainScreen].bounds.size.width;
   }
   
   host_->resize(width_, height_, scale_);
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update {
   host_->update(self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
   host_->render();
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
   Moves moves;
   
   for (UITouch *uiTouch in touches) {
      CGPoint cur = [uiTouch locationInView:self.view];
      CGPoint pre = [uiTouch previousLocationInView:self.view];
      moves.push_back(Move(cur.x, cur.y, pre.x, pre.y));
   }
   
   host_->touchDown(moves);
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
   Moves moves;
   
   for (UITouch *uiTouch in touches) {
      CGPoint cur = [uiTouch locationInView:self.view];
      CGPoint pre = [uiTouch previousLocationInView:self.view];
      moves.push_back(Move(cur.x, cur.y, pre.x, pre.y));
   }
   
   host_->touchUp(moves);
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
   Moves moves;
   
   for (UITouch *uiTouch in touches) {
      CGPoint cur = [uiTouch locationInView:self.view];
      CGPoint pre = [uiTouch previousLocationInView:self.view];
      moves.push_back(Move(cur.x, cur.y, pre.x, pre.y));
   }
   
   host_->touchMove(moves);
}

@end
