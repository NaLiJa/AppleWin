//
//  PrinterView.m
//  Mariani
//
//  Created by sh95014 on 3/12/22.
//

#import "PrinterView.h"

@interface PrinterString : NSObject
@property (strong) NSString *string;
@property (assign) CGPoint location;
@end

@implementation PrinterString
#ifdef DEBUG
- (NSString *)description {
    return [NSString stringWithFormat:@"%@ (%.1f, %.1f) \"%@\"", [super description], self.location.x, self.location.y, self.string];
}
#endif // DEBUG
@end

@interface PrinterPage : NSObject
@property (strong) NSMutableArray<PrinterString *> *strings;
@end

@implementation PrinterPage
@end

@interface PrinterView ()

@property (strong) NSFont *font;
@property (assign) CGFloat lineHeight;
@property (strong) NSDictionary *fontAttributes;
@property (strong) NSMutableArray<PrinterPage *> *pages;

@end

@implementation PrinterView

- (void)awakeFromNib {
    PrinterPage *page = [[PrinterPage alloc] init];
    page.strings = [NSMutableArray array];
    self.pages = [NSMutableArray arrayWithObject:page];
    
    self.font = [NSFont fontWithName:@"FXMatrix105MonoEliteRegular" size:9];
    self.lineHeight = self.font.ascender + self.font.descender + self.font.leading;
    
    // To match the resolution of AppleWriterPrinter, we assume 72 dpi and
    // the default Elite font is 12 cpi, so we want our character spacing to
    // fit 12 characters per "inch" on the screen.
    const CGFloat fontWidth = self.font.maximumAdvancement.width;
    const CGFloat characterWidth = (self.bounds.size.width / 8.5) / 12.0;
    
    // But unfortunately, that math seems to be slightly off, possibly because
    // maximumAdvancement is lying, so let's fudge  it based on real
    // measurements:
    const CGFloat kerning = (characterWidth - fontWidth) * 0.925;
    
    self.fontAttributes = @{
        NSFontAttributeName: self.font,
        NSKernAttributeName: @(kerning),
    };
}

- (BOOL)isFlipped {
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    const BOOL isDrawingToScreen = [NSGraphicsContext currentContextDrawingToScreen];
    
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);
    
    PrinterPage *page;
    if (isDrawingToScreen) {
        page = [self.pages lastObject];
    }
    else {
        NSInteger pageNumber = [[NSPrintOperation currentOperation] currentPage];
        page = [self.pages objectAtIndex:pageNumber - 1];
    }
    for (PrinterString *ps in page.strings) {
        CGPoint location = ps.location;
        // BasePrinter's coordinates are for the top-left corner of the
        // character
        if (isDrawingToScreen) {
            location.y += self.lineHeight;
        }
        else {
            // no idea why this fudge factor is needed but otherwise it's too
            // high on the page
            location.y += self.lineHeight * 2.2;
        }
        [ps.string drawAtPoint:location withAttributes:self.fontAttributes];
    }
}

- (NSRect)rectForPage:(NSInteger)page {
    return [self bounds];
}

- (BOOL)knowsPageRange:(NSRangePointer)range {
    *range = NSMakeRange(1, self.pages.count);
    return YES;
}

- (void)addString:(NSString *)string atPoint:(CGPoint)location {
    PrinterString *printerString = [[PrinterString alloc] init];
    printerString.string = string;
    printerString.location = location;
    PrinterPage *page = [self.pages lastObject];
    [page.strings addObject:printerString];
    
    [self setNeedsDisplay:YES];
}

- (void)addPage {
    PrinterPage *page = [[PrinterPage alloc] init];
    page.strings = [NSMutableArray array];
    [self.pages addObject:page];
    
    [self setNeedsDisplay:YES];
}

@end
