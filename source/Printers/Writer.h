// Copyright 2017 Nick Westgate
//
// This file is part of the Ancient Printer Emulation Libarary (APEL).
//
// APEL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// APEL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with APEL. If not, see <http://www.gnu.org/licenses/>.

#pragma once

namespace AncientPrinterEmulationLibrary
{
    class Writer
    {
    public:
        Writer() {};
        virtual ~Writer() { Close(); };

        virtual void Close() {};
        virtual void EndPage() {};
        virtual int  Plot(int x, int y) { return 0; };
        virtual int  SetFont(int textWidth, int textHeight) { return 0; };
        virtual int  SetPageMetrics(int pageWidth, int pageHeight, int dotSize) { return 0; };
        virtual int  WriteCharacter(int x, int y, char character, bool isAdjacent = false) { return 0; };

    private:
        Writer(Writer const&);
    };
}
