//
//  Rasterizer.java
//
//  Created by Warren R. Carithers on 2016/09/30.
//  Based on code created by Joe Geigel on 1/21/10.
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
    ///
    Rasterizer( int n, Canvas canvas )
    {
        n_scanlines = n;
	C = canvas;
    }
    
    ///
    // Draw a filled polygon in the Canvas C.
    //
    // The polygon has n distinct vertices. The coordinates of the
    // vertices making up the polygon are stored in the x and y arrays.
    // The ith vertex will have coordinate  (x[i], y[i])
    //
    // You are to add the implementation here using only calls to C.setPixel()
    //
    // @param n number of vertices
    // @param x x coordinates
    // @param y y coordinates
    ///
    public void drawPolygon( int n, int x[], int y[] )
    {
        // YOUR IMPLEMENTATION HERE

    }
}
