//
//  Rasterizer.java
//
//  Created by Warren R. Carithers on 2016/09/23.
//  Based on code created by Joe Geigel on 2011/11/30.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

///
// 
// A simple class for performing rasterization algorithms.
//
///

import java.util.*;

public class Rasterizer {
    
    ///
    // number of scanlines
    ///
    private int n_scanlines;
    
    ///
    // Drawing canvas
    ///
    public Canvas C;

    ///
    // Constructor
    //
    // @param n number of scanlines
    // @param C drawing canvas to use
    //
    ///

    Rasterizer( int n, Canvas canvas )
    {
        n_scanlines = n;
	C = canvas;
    }

    ///
    // Draw my initials
    //
    // Draw my own initials using calls to drawLine(), in the same
    // manner that makeLines() in the driver program draws 'CG'.
    //
    ///
    void myInitials()
    {

        // ######## Use blue color (0,0.5,1) to write your initials ########

        C.setColor( 0.0f, 0.5f, 1.0f );

	//
	// add code here to draw your initials
	// with calls to your drawLine() function


    }

    ///
    // Draw a line from (x0,y0) to (x1, y1) on the simpleCanvas C.
    //
    // Implementation should be using the Midpoint Method
    //
    // You are to add the implementation here using only calls
    // to C.setPixel()
    //
    // @param x0 x coord of first endpoint
    // @param y0 y coord of first endpoint
    // @param x1 x coord of second endpoint
    // @param y1 y coord of second endpoint
    ///

    public void drawLine( int x0, int y0, int x1, int y1 )
    {
        //
	// add code here to implement drawLine()
	// using the midpoint line algorithm
	//

    }

}
