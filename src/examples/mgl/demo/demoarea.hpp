/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:     C++ 3.0
* Environment:  any
*
* Description:  Header file for the class DemoArea, a class for
*               performing the demos in a double buffered window.
*
****************************************************************************/

#ifndef __DEMOAREA_HPP
#define __DEMOAREA_HPP

#include "mvis/mrendera.hpp"

/*-------------------------- Class definitions ----------------------------*/

//---------------------------------------------------------------------------
// The DemoArea class is a class used to represent an area for peforming
// the double buffered demonstrations.
//---------------------------------------------------------------------------

class DemoArea : public MVRenderArea {
protected:
	ibool       inDemo;             // True if currently running demo
    MGLDevCtx   background;         // Background bitmap for window
	int			defPalSize;         // Size of default palette
    palette_t   defPal[256];        // Buffer for default palette

            // Methods to begin and end the demos
            void beginDemo();
            void endDemo();

            // Method to run each of the specific demonstrations
            void displayStatusInfo();
            void lineDemo();
            void lineSpeedDemo();
            void lineNormalDemo();
            void ellipseDemo();
            void arcDemo();
            void polygonDemo();
            void polyDemo();
            void convexPolyDemo();
            void cShadedPolyDemo();
			void rgbShadedPolyDemo();
			void solidRectangleDemo();
			void opaquePattRectangleDemo();
			void transPattRectangleDemo();
			void colorPattRectangleDemo();
			void outlineRectangleDemo();
			void rectangleDemo();
			void fontDemo();
            void colorDemo();
			void patternDemo();
			void boundaryFillDemo();
            void interiorFillDemo();
            void animationDemo();

            // Run the specified demo
            void runDemo(int demo);

            // Method to set the demonstration title name
            void setDemoTitle(char *title);

            // Method to set the information window text
            void setInfoText(char *text);

public:
            // Constructor
            DemoArea(MGLDevCtx& dc,const MVRect& bounds);

            // Method handle interaction with the rendering
    virtual void handleEvent(MVEvent& event,phaseType phase = phFocused);

            // Method to render the scene
    virtual void render();
    };

#endif  // __RENDERA_HPP
