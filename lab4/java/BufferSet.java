//
//  BufferSet.java
//
//  Simple representation of a "Vertex".
//
//  Author:  Warren R. Carithers
//  Date:    2016/10/11 12:45:54
//
//  This class is an example of an anomaly in Java, as it
//  has data members but no member functions (other than a
//  constructor for ease of use).  It is, in essence, a
//  record/struct.
//
//  Java purists who insist that this is a heinous crime
//  should refer to the Wikipedia description of "PODS",
//  <https://en.wikipedia.org/wiki/Plain_old_data_structure>.
//
//  Not everything needs to be "smart".
//

public class BufferSet {

    ///
    // canvas size information
    ///
    public int vbuffer;         // buffer handles
    public int ebuffer;
    public int numElements;     // total number of vertices
    public long vSize;          // component sizes (bytes)
    public long eSize;
    public long cSize;
    public boolean bufferInit;  // have these already been set up?

    ///
    // Constructor
    ///
    public BufferSet()
    {
        vbuffer = 0;
        ebuffer = 0;
        numElements = 0;
	vSize = 0;
	eSize = 0;
	cSize = 0;
	bufferInit = false;
    }
}
