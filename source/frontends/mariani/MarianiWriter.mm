//
//  MarianiWriter.cpp
//  Mariani
//
//  Created by sh95014 on 3/12/22.
//

#import <Cocoa/Cocoa.h>
#import "PrinterView.h"
#import "MarianiWriter.h"

// We want to collate the incoming characters into strings so that the printer
// view isn't overwhelmed with objects, but that requires it to fudge kerning.
// Disable COLLATE_STRINGS to get back to the per-character behavior to check
// for accuracy.
#define COLLATE_STRINGS

namespace AncientPrinterEmulationLibrary
{
    MarianiWriter::MarianiWriter(PrinterView *printerView) :
        myPrinterView(printerView)
    {
        
    }

    int MarianiWriter::WriteCharacter(int x, int y, char character, bool isAdjacent)
    {
        if (myPrinterView) {
#ifdef COLLATE_STRINGS
            if (!isAdjacent)
#endif
            {
                // output the string accumulated so far to the Writer
                if (string.length > 0) {
                    [myPrinterView addString:string atPoint:CGPointMake((CGFloat)stringX / 20.0, (CGFloat)stringY / 20)];
                }
                string = nil;
            }
            if (string == nil) {
                string = [NSString stringWithFormat:@"%c", character];
                stringX = x;
                stringY = y;
            }
            else {
                string = [string stringByAppendingFormat:@"%c", character];
            }
            
            // BasePrinter assumes its own width and ignores the return
            // value, so don't bother computing yet. A small positive number
            // should ensure ugly overlaps if we're paired with a Printer that
            // actually does.
            return 3;
        }
        return 0;
    }
}
