//
//  ColorView.m
//  ofxRemoteUIClientOSX
//
//  Created by Oriol Ferrer Mesià on 17/05/13.
//
//

#import "ColorView.h"

@implementation ColorView


-(void)dealloc{
	[background release];
	[super dealloc];
}

-(void)awakeFromNib{
	background = [[NSColor clearColor] retain];
}


-(BOOL)isOpaque{
	return YES;
}

- (BOOL)wantsDefaultClipping {
	return NO;
}

-(void)setBackgroundColor:(NSColor *)aColor{

    if([background isEqual:aColor]) return;
    [background release];
    background = [aColor retain];

    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect{
	[background set];
    NSRectFill(dirtyRect);
}

@end
